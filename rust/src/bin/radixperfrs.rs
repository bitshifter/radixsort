extern crate num;
extern crate rand;
extern crate sortrs;
extern crate time;
extern crate radixsort;

use num::FromPrimitive;
use radixsort::{ radix8sort_u32, radix8sort_u64, radix8sort_f32,
	radix11sort_u32, radix11sort_u64, radix11sort_f32 };
use rand::{ weak_rng, Rng, Rand };
use std::iter::{ repeat };
use std::vec::Vec;
use time::precise_time_s;
use sortrs::{ introsort_by };

#[macro_use]
mod helpers;

fn perf_test<T: Rand + Clone + PartialOrd + FromPrimitive,
F: Fn(&mut[T], &mut[T], &mut[u32], &mut[u32]) -> usize,
G: Fn(&mut[T], &mut[T], &mut[u32], &mut[u32]) -> usize>(
	radixsort8: F, radixsort11: G, size: usize, iterations: usize)
{
	let mut rng = weak_rng();
	let mut radix8_total = 0f64;
	let mut radix11_total = 0f64;
	let mut stdsort_total = 0f64;
	let mut introsort_total = 0f64;
	for _ in (0..iterations)
	{
		let keys_orig: Vec<T> = (0..size).map(|_| rng.gen()).collect();
		let values_orig: Vec<u32> = (0..size).map(|i| i as u32).collect();
		{
			let mut keys0 = keys_orig.clone();
			let mut keys1: Vec<T>  = repeat(FromPrimitive::from_usize(0).unwrap()).take(size).collect();
			let mut values0 = values_orig.clone();
			let mut values1: Vec<u32> = repeat(0u32).take(size).collect();
			let start_time = precise_time_s();
			let passes = radixsort8(&mut keys0, &mut keys1, &mut values0, &mut values1);
			radix8_total += precise_time_s() - start_time;
			match passes & 1
			{
				0 => check_sorted!(keys0, values0, keys_orig),
				_ => check_sorted!(keys1, values1, keys_orig)
			}
		}

		{
			let mut keys0 = keys_orig.clone();
			let mut keys1: Vec<T> = repeat(FromPrimitive::from_usize(0).unwrap()).take(size).collect();
			let mut values0 = values_orig.clone();
			let mut values1: Vec<u32> = repeat(0u32).take(size).collect();
			let start_time = precise_time_s();
			let passes = radixsort11(&mut keys0, &mut keys1, &mut values0, &mut values1);
			radix11_total += precise_time_s() - start_time;
			match passes & 1
			{
				0 => check_sorted!(keys0, values0, keys_orig),
				_ => check_sorted!(keys1, values1, keys_orig)
			}
		}

		{
			let it = keys_orig.iter().zip(values_orig.iter());
			let (lower, _) = it.size_hint();
			let mut pairs = Vec::with_capacity(lower);

			for (key, value) in it
			{
				pairs.push((key, value));
			}
			let start_time = precise_time_s();
			pairs.sort_by(|a,b| {
				let (ka, _) = *a;
				let (kb, _) = *b;
				ka.partial_cmp(kb).unwrap()
			});
			stdsort_total += precise_time_s() - start_time;
		}

		{
			let it = keys_orig.iter().zip(values_orig.iter());
			let (lower, _) = it.size_hint();
			let mut pairs = Vec::with_capacity(lower);

			for (key, value) in it
			{
				pairs.push((key, value));
			}
			let start_time = precise_time_s();
			sortrs::introsort_by(&mut pairs, |a, b| {
				let (ka, _) = *a;
				let (kb, _) = *b;
				ka.lt(kb)
			});
			introsort_total += precise_time_s() - start_time;
		}
	}

	let mult = 1.0 / iterations as f64;
	println!("{:6}  {:.6}  {:.6}  {:.6}  {:.6}", size, radix8_total * mult,
		radix11_total * mult, introsort_total * mult, stdsort_total * mult);
}

fn main()
{
	let start = 32;
	let end = 65536;
	let iterations = 100;

	println!("Radix sort u32 key ({} iterations)", iterations);
	println!("  size    radix8   radix11 introsort   stdsort");

	let mut i = start;
	while i <= end
	{
		perf_test(radix8sort_u32, radix11sort_u32, i, iterations);
		i = i << 1;
	}

	println!("\nRadix sort u64 key ({} iterations)", iterations);
	println!("  size    radix8   radix11 introsort   stdsort");

	let mut i = start;
	while i <= end
	{
		perf_test(radix8sort_u64, radix11sort_u64, i, iterations);
		i = i << 1;
	}

	println!("\nRadix sort f32 key ({} iterations)", iterations);
	println!("  size    radix8   radix11 introsort   stdsort");

	let mut i = start;
	while i <= end
	{
		perf_test(radix8sort_f32, radix11sort_f32, i, iterations);
		i = i << 1;
	}
}

