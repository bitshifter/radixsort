extern crate rand;
extern crate time;
extern crate radixsort;

use helpers::{ check_sorted };
use radixsort::{ radix8sort_u32, radix8sort_u64, radix8sort_f32,
	radix11sort_u32, radix11sort_u64, radix11sort_f32 };
use std::num::from_uint;
use std::rand::{ weak_rng, Rng, Rand };
use std::vec::Vec;
use time::precise_time_s;

mod helpers;

fn perf_test<T: Rand + Clone + PartialOrd + FromPrimitive>(
	radixsort8: |&mut[T], &mut[T], &mut[u32], &mut[u32]| -> uint,
	radixsort11: |&mut[T], &mut[T], &mut[u32], &mut[u32]| -> uint,
	size: uint, iterations: uint)
{
	let mut rng = weak_rng();
	let mut radix8_total = 0f64;
	let mut radix11_total = 0f64;
	let mut stdvec_total = 0f64;
	for _ in range(0, iterations)
	{
		let keys_orig: Vec<T> = Vec::from_fn(size, |_| rng.gen());
		let values_orig = Vec::from_fn(size, |i| i as u32);
		{
			let mut keys0 = keys_orig.clone();
			let mut keys1 = Vec::from_elem(size, from_uint::<T>(0).unwrap());
			let mut values0 = values_orig.clone();
			let mut values1 = Vec::from_elem(size, 0u32);
			let start_time = precise_time_s();
			let passes = radixsort8(keys0.as_mut_slice(), keys1.as_mut_slice(),
				values0.as_mut_slice(), values1.as_mut_slice());
			radix8_total += precise_time_s() - start_time;
			match passes & 1
			{
				0 => check_sorted(keys0.as_slice(), values0.as_slice(),
					keys_orig.as_slice()),
				_ => check_sorted(keys1.as_slice(), values1.as_slice(),
					keys_orig.as_slice())
			}
		}

		{
			let mut keys0 = keys_orig.clone();
			let mut keys1 = Vec::from_elem(size, from_uint::<T>(0).unwrap());
			let mut values0 = values_orig.clone();
			let mut values1 = Vec::from_elem(size, 0u32);
			let start_time = precise_time_s();
			let passes = radixsort11(keys0.as_mut_slice(), keys1.as_mut_slice(),
				values0.as_mut_slice(), values1.as_mut_slice());
			radix11_total += precise_time_s() - start_time;
			match passes & 1
			{
				0 => check_sorted(keys0.as_slice(), values0.as_slice(),
					keys_orig.as_slice()),
				_ => check_sorted(keys1.as_slice(), values1.as_slice(),
					keys_orig.as_slice())
			}
		}

		{
			let mut it = keys_orig.iter().zip(values_orig.iter());
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
			stdvec_total += precise_time_s() - start_time;
		}
	}
	let mult = 1.0 / iterations as f64;
	println!("{:6}  {:.6}  {:.6}  {:.6}", size, radix8_total * mult,
		radix11_total * mult, stdvec_total * mult);
}

fn main()
{
	let start = 32;
	let end = 65536;
	let iterations = 100;

	println!("Radix sort u32 key ({} iterations)", iterations);
	println!("  size    radix8   radix11 Vec::sort");

	let mut i = start;
	while i <= end
	{
		perf_test::<u32>(radix8sort_u32, radix11sort_u32, i, iterations);
		i = i << 1;
	}

	println!("\nRadix sort u64 key ({} iterations)", iterations);
	println!("  size    radix8   radix11 Vec::sort");

	let mut i = start;
	while i <= end
	{
		perf_test::<u64>(radix8sort_u64, radix11sort_u64, i, iterations);
		i = i << 1;
	}

	println!("\nRadix sort f32 key ({} iterations)", iterations);
	println!("  size    radix8   radix11 Vec::sort");

	let mut i = start;
	while i <= end
	{
		perf_test::<f32>(radix8sort_f32, radix11sort_f32, i, iterations);
		i = i << 1;
	}
}

