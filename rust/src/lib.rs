/*
 * Copyright (c) 2014 Cameron Hart
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#![allow(dead_code)]

use std::mem;

/**
 * Flip a float for sorting.
 *  finds SIGN of fp number.
 *  if it's 1 (negative float), it flips all bits
 *  if it's 0 (positive float), it flips the sign only
 */
fn float_flip(f: u32) -> u32
{
	let mask = (-((f >> 31) as i32) as u32) | 0x80000000;
	f ^ mask
}


/**
 * Flip a float back (invert floatFlip)
 *  signed was flipped from above, so:
 *  if sign is 1 (negative), it flips the sign bit back
 *  if sign is 0 (positive), it flips all bits back
 */
fn inv_float_flip(f: u32) -> u32
{
	let mask = ((f >> 31) - 1) | 0x80000000;
	f ^ mask
}


fn sum_histograms(hist: &mut[usize], sum: &mut[usize],
	hist_buckets: usize, hist_size: usize)
{
	// Update the histogram data so each entry sums the previous entries
	for bucket in (0..hist_buckets)
	{
		let histindex = bucket * hist_size;
		sum[bucket] = hist[histindex];
		hist[histindex] = 0;
	}

	let mut tsum : usize;
	for i in (1..hist_size)
	{
		for bucket in (0..hist_buckets)
		{
			let histindex = bucket * hist_size + i;
			tsum = hist[histindex] + sum[bucket];
			hist[histindex] = sum[bucket];
			sum[bucket] = tsum;
		}
	}
}


macro_rules! radix_pass(
	($key_ty:ty, $keys_in:ident, $keys_out:ident, $values_in:ident, $values_out:ident, $hist:ident, $shift:ident, $mask:ident) => {{
		let size = $keys_in.len();
		for i in 0..size
		{
			let key = $keys_in[i];
			let pos = ((key >> $shift) & $mask) as usize;
			let index = $hist[pos];
			$hist[pos] += 1;
			$keys_out[index] = key;
			// TODO: is there a better general purpose way of copying?
			mem::swap(&mut $values_out[index], &mut $values_in[i]);
		}
	}}
);


/*
fn radix_pass<K: Unsigned + Integer + FromPrimitive + ToPrimitive, V: Clone>(
	keys_in: &[K], keys_out: &mut[K],
	values_in: &[V], values_out: &mut[V],
	hist: &mut[usize], shift: usize, mask: usize)
{
	let size = keys_in.len();
	for i in (0..size)
	{
		let key = ToPrimitive::to_usize(&keys_in[i]).unwrap();
		let pos = (key >> shift) & mask;
		let index = hist[pos];
		hist[pos] += 1;
		keys_out[index] = FromPrimitive::from_usize(key).unwrap();
		values_out[index] = values_in[i].clone();
	}
}
*/


fn radix_pass_decode_float<V: Clone>(
	keys_in: &[u32], keys_out: &mut[u32],
	values_in: &[V], values_out: &mut[V],
	hist: &mut[usize], shift: usize, mask: u32)
{
	let size = keys_in.len();
	for i in (0..size)
	{
		let key = float_flip(keys_in[i]) as usize;
		let pos = (key >> shift as usize) & mask as usize;
		let index = hist[pos];
		hist[pos] += 1;
		keys_out[index] = key as u32;
		values_out[index] = values_in[i].clone();
	}
}


fn radix_pass_encode_float<V: Clone>(
	keys_in: &[u32], keys_out: &mut[u32],
	values_in: &[V], values_out: &mut[V],
	hist: &mut[usize], shift: usize, mask: u32)
{
	let size = keys_in.len();
	for i in (0..size)
	{
		let key = keys_in[i];
		let pos = ((key >> shift as usize) & mask) as usize;
		let index = hist[pos];
		hist[pos] += 1;
		keys_out[index] = inv_float_flip(key);
		values_out[index] = values_in[i].clone();
	}
}


macro_rules! radix_sort_uint {
	($key_ty:ty, $radix_bits:expr, $key_bits:expr, $keys_in:ident, $keys_temp:ident, $values_in:ident, $values_temp:ident) =>
	{{
		const RADIX_BITS: usize = $radix_bits;
		const KEY_BITS: usize = $key_bits;
		const HIST_BUCKETS: usize = 1 + (KEY_BITS - 1) / RADIX_BITS;
		const HIST_SIZE: usize = 1 << RADIX_BITS;
		const HIST_MASK: $key_ty = (HIST_SIZE - 1) as $key_ty;
		assert_eq!(KEY_BITS, mem::size_of::<$key_ty>() * 8);

		let mut hist = [0; HIST_SIZE * HIST_BUCKETS];
		let mut sum = [0; HIST_BUCKETS];

		assert_eq!($keys_in.len(), $values_in.len());
		assert_eq!($keys_in.len(), $keys_temp.len());
		assert_eq!($values_in.len(), $values_temp.len());

		for key in $keys_in.iter()
		{
			for bucket in 0..HIST_BUCKETS
			{
				let key_shift = bucket * RADIX_BITS;
				let pos = (*key >> key_shift) & HIST_MASK as $key_ty;
				let index = bucket * HIST_SIZE + pos as usize;
				hist[index] += 1;
			}
		}

		sum_histograms(&mut hist, &mut sum, HIST_BUCKETS, HIST_SIZE);

		let mut key_bits = 0;
		let mut i0 = 0;
		let mut i1 = HIST_SIZE;
		for i in 0..HIST_BUCKETS
		{
			let bucket = &mut hist[i0..i1];
			match i & 1
			{
				0 => radix_pass!($key_ty, $keys_in, $keys_temp, $values_in, $values_temp, bucket,
								 key_bits, HIST_MASK),
				_ => radix_pass!($key_ty, $keys_temp, $keys_in, $values_temp, $values_in, bucket,
								 key_bits, HIST_MASK)
			}
			i0 += HIST_SIZE;
			i1 += HIST_SIZE;
			key_bits = key_bits + RADIX_BITS;
		}
		HIST_BUCKETS
	}};
}


macro_rules! radix_sort_float(
	($radix_bits:expr, $keys_in:ident, $keys_temp:ident, $values_in:ident, $values_temp:ident) => {{
		const RADIX_BITS: usize = $radix_bits;
		const KEY_BITS: usize = 4 * 8;
		const HIST_BUCKETS: usize = 1 + (KEY_BITS - 1) / RADIX_BITS;
		const HIST_SIZE: usize = 1 << RADIX_BITS;
		const HIST_MASK: u32 = (HIST_SIZE - 1) as u32;
		assert_eq!(KEY_BITS, mem::size_of::<f32>() * 8);

		let mut hist = [0; HIST_SIZE * HIST_BUCKETS];
		let mut sum = [0; HIST_BUCKETS];

		let keys_in: &mut[u32] = unsafe { mem::transmute($keys_in) };
		let keys_temp: &mut[u32] = unsafe { mem::transmute($keys_temp) };

		assert_eq!(keys_in.len(), $values_in.len());
		assert_eq!(keys_in.len(), keys_temp.len());
		assert_eq!($values_in.len(), $values_temp.len());

		for key_ref in keys_in.iter()
		{
			for bucket in 0..HIST_BUCKETS
			{
				let key = float_flip(*key_ref);
				let key_shift = bucket * RADIX_BITS;
				let pos = ((key >> key_shift) & HIST_MASK) as usize;
				let index = bucket * HIST_SIZE + pos;
				hist[index] += 1;
			}
		}

		sum_histograms(&mut hist, &mut sum, HIST_BUCKETS, HIST_SIZE);

		let mut key_bits = 0;
		let mut i0 = 0;
		let mut i1 = HIST_SIZE;

		{
			let bucket = &mut hist[i0..i1];
			radix_pass_decode_float(keys_in, keys_temp, $values_in, $values_temp, bucket,
				key_bits, HIST_MASK);
			key_bits += RADIX_BITS;
			i0 += HIST_SIZE;
			i1 += HIST_SIZE;
		}

		for i in 1..HIST_BUCKETS - 1
		{
			let bucket = &mut hist[i0..i1];
			match i & 1
			{
				0 => radix_pass!(u32, keys_in, keys_temp, $values_in, $values_temp, bucket,
						key_bits, HIST_MASK),
				_ => radix_pass!(u32, keys_temp, keys_in, $values_temp, $values_in, bucket,
						key_bits, HIST_MASK)
			}
			key_bits += RADIX_BITS;
			i0 += HIST_SIZE;
			i1 += HIST_SIZE;
		}

		{
			let bucket = &mut hist[i0..i1];
			match (HIST_BUCKETS - 1) & 1
			{
				0 => radix_pass_encode_float(keys_in, keys_temp, $values_in, $values_temp, bucket,
						key_bits, HIST_MASK),
				_ => radix_pass_encode_float(keys_temp, keys_in, $values_temp, $values_in, bucket,
						key_bits, HIST_MASK)
			}
		}

		HIST_BUCKETS
	}}
);


pub fn radix8sort_u64<V: Clone>(
	keys_in: &mut[u64], keys_temp: &mut[u64],
	values_in: &mut[V], values_temp: &mut[V]) -> usize
{
	return radix_sort_uint!(u64, 8, 64, keys_in, keys_temp, values_in, values_temp);
}


pub fn radix8sort_u32<V: Clone>(
	keys_in: &mut[u32], keys_temp: &mut[u32],
	values_in: &mut[V], values_temp: &mut[V]) -> usize
{
	return radix_sort_uint!(u32, 8, 32, keys_in, keys_temp, values_in, values_temp);
}


pub fn radix8sort_f32<V: Clone>(
	fkeys_in: &mut[f32], fkeys_temp: &mut[f32],
	values_in: &mut[V], values_temp: &mut[V]) -> usize
{
	return radix_sort_float!(8, fkeys_in, fkeys_temp, values_in, values_temp);
}


pub fn radix11sort_u64<V: Clone>(
	keys_in: &mut[u64], keys_temp: &mut[u64],
	values_in: &mut[V], values_temp: &mut[V]) -> usize
{
	return radix_sort_uint!(u64, 11, 64, keys_in, keys_temp, values_in, values_temp);
}


pub fn radix11sort_u32<V: Clone>(
	keys_in: &mut[u32], keys_temp: &mut[u32],
	values_in: &mut[V], values_temp: &mut[V]) -> usize
{
	return radix_sort_uint!(u32, 11, 32, keys_in, keys_temp, values_in, values_temp);
}


pub fn radix11sort_f32<V: Clone>(
	fkeys_in: &mut[f32], fkeys_temp: &mut[f32],
	values_in: &mut[V], values_temp: &mut[V]) -> usize
{
	return radix_sort_float!(11, fkeys_in, fkeys_temp, values_in, values_temp);
}

