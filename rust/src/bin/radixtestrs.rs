extern crate radixsort;
extern crate rand;

use helpers::{ check_sorted };
use radixsort::{ radix8sort_u32, radix8sort_u64, radix8sort_f32, radix11sort_u32,
	radix11sort_u64, radix11sort_f32 };
use std::iter::{ repeat, range };
use std::num::from_uint;
use std::num::FromPrimitive;
use std::rand::{ weak_rng, Rng, Rand };
use std::vec::Vec;

mod helpers;

trait PrintArrayElem
{
	fn print(&self);
}

impl PrintArrayElem for u32
{
	fn print(&self)
	{
		print!("{:08x} ", *self);
	}
}

impl PrintArrayElem for u64
{
	fn print(&self)
	{
		print!("{:016x} ", *self);
	}
}

impl PrintArrayElem for f32
{
	fn print(&self)
	{
		print!("{} ", *self);
	}
}

fn print_array<Key: PrintArrayElem>(keys: &[Key])
{
	for key in keys.iter()
	{
		key.print();
	}
	println!("");
}

fn test_radix<T: Clone + PartialOrd + FromPrimitive + Rand + PrintArrayElem>(
	func: |&mut[T], &mut[T], &mut[u32], &mut[u32]| -> uint, size: uint)
{
	let mut rng = weak_rng();
	let keys_orig: Vec<T> = range(0, size).map(|_| rng.gen::<T>()).collect();
	let values_orig: Vec<u32> = range(0, size).map(|i| i as u32).collect();
	let mut keys_in_out = keys_orig.clone();
	let mut keys_temp: Vec<T> = repeat(from_uint::<T>(0).unwrap()).take(size).collect();
	let mut values_in_out = values_orig.clone();
	let mut values_temp: Vec<u32> = repeat(0u32).take(size).collect();
	print_array(keys_in_out.as_slice());
	let passes = func(keys_in_out.as_mut_slice(), keys_temp.as_mut_slice(),
		values_in_out.as_mut_slice(), values_temp.as_mut_slice());
	match passes & 1
	{
		0 => { 
			print_array(keys_in_out.as_slice());
			check_sorted(keys_in_out.as_slice(), values_in_out.as_slice(),
				keys_orig.as_slice());
		},
		_ => {
			print_array(keys_temp.as_slice());
			check_sorted(keys_temp.as_slice(), values_temp.as_slice(),
				keys_orig.as_slice());
		}
	}
}

fn main()
{
	const SIZE: uint = 8;
	println!("radix8sort u32");
	test_radix(radix8sort_u32, SIZE);
	println!("radix8sort u64");
	test_radix(radix8sort_u64, SIZE);
	println!("radix8sort f32");
	test_radix(radix8sort_f32, SIZE);
	println!("radix11sort u32");
	test_radix(radix11sort_u32, SIZE);
	println!("radix11sort u64");
	test_radix(radix11sort_u64, SIZE);
	println!("radix11sort f32");
	test_radix(radix11sort_f32, SIZE);
}
