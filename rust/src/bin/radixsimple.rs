extern crate radixsort;
extern crate time;

use radixsort::{ radix8sort_u64 };
use std::iter::{ repeat };
use std::rand::{ weak_rng, Rng };
use std::vec::Vec;
use time::precise_time_s;

fn perf_test(size: usize, iterations: usize)
{
	let mut rng = weak_rng();
	let mut radix8_total = 0f64;
	for _ in (0..iterations)
	{
		let mut keys0: Vec<u64> = (0..size).map(|_| rng.gen()).collect();
		let mut keys1: Vec<u64> = repeat(0u64).take(size).collect();
		let mut values0: Vec<u32> = (0..size).map(|i| i as u32).collect();
		let mut values1: Vec<u32> = repeat(0u32).take(size).collect();
		let start_time = precise_time_s();
		radix8sort_u64(keys0.as_mut_slice(), keys1.as_mut_slice(),
			values0.as_mut_slice(), values1.as_mut_slice());
		radix8_total += precise_time_s() - start_time;
	}
	let mult = 1.0 / iterations as f64;
	println!("size: {:6} iterations: {}  avg: {:.6}", size, iterations, radix8_total * mult);
}

fn main() {
	perf_test(65535, 100);
}
