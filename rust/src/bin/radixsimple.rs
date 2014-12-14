extern crate radixsort;
extern crate time;

use radixsort::{ radix8sort_u64 };
use std::rand::{ weak_rng, Rng };
use std::vec::Vec;
use time::precise_time_s;

fn perf_test(size: uint, iterations: uint)
{
	let mut rng = weak_rng();
	let mut radix8_total = 0f64;
	for _ in range(0, iterations)
	{
		let mut keys0: Vec<u64> = Vec::from_fn(size, |_| rng.gen());
		let mut keys1 = Vec::from_elem(size, 0u64);
		let mut values0 = Vec::from_fn(size, |i| i as u32);
		let mut values1 = Vec::from_elem(size, 0u32);
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
