#![allow(dead_code)]

use std::mem::transmute;
use std::mem::size_of;
use std::num::one;
use std::num::from_uint;


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


fn sum_histograms(hist: &mut[uint], sum: &mut[uint],
	hist_buckets: uint, hist_size: uint)
{
	// Update the histogram data so each entry sums the previous entries
	for bucket in range(0, hist_buckets)
	{
		let histindex = bucket * hist_size;
		sum[bucket] = hist[histindex];
		hist[histindex] = 0;
	}

	let mut tsum : uint;
	for i in range(1, hist_size)
	{
		for bucket in range(0, hist_buckets)
		{
			let histindex = bucket * hist_size + i;
			tsum = hist[histindex] + sum[bucket];
			hist[histindex] = sum[bucket];
			sum[bucket] = tsum;
		}
	}
}


fn radix_pass<K: Unsigned + Int, V: Clone>(
	keys_in: &[K], keys_out: &mut[K],
	values_in: &[V], values_out: &mut[V],
	hist: &mut[uint], shift: uint, mask: K)
{
	let size = keys_in.len();
	for i in range(0, size)
	{
		let key = keys_in[i];
		let pos = ((key >> shift) & mask).to_uint().unwrap();
		let index = hist[pos];
		hist[pos] += 1;
		keys_out[index] = key;
		values_out[index] = values_in[i].clone();
	}
}


fn radix_pass_decode_float<V: Clone>(
	keys_in: &[u32], keys_out: &mut[u32],
	values_in: &[V], values_out: &mut[V],
	hist: &mut[uint], shift: uint, mask: u32)
{
	let size = keys_in.len();
	for i in range(0, size)
	{
		let key = float_flip(keys_in[i]) as uint;
		let pos = (key >> shift as uint) & mask as uint;
		let index = hist[pos];
		hist[pos] += 1;
		keys_out[index] = key as u32;
		values_out[index] = values_in[i].clone();
	}
}


fn radix_pass_encode_float<V: Clone>(
	keys_in: &[u32], keys_out: &mut[u32],
	values_in: &[V], values_out: &mut[V],
	hist: &mut[uint], shift: uint, mask: u32)
{
	let size = keys_in.len();
	for i in range(0, size)
	{
		let key = keys_in[i];
		let pos = ((key >> shift as uint) & mask) as uint;
		let index = hist[pos];
		hist[pos] += 1;
		keys_out[index] = inv_float_flip(key);
		values_out[index] = values_in[i].clone();
	}
}


fn radix_sort_uint<K: Unsigned + Int + FromPrimitive, V: Clone>(radix_bits: uint,
	keys_in: &mut[K], keys_temp: &mut[K],
	values_in: &mut[V], values_temp: &mut[V],
	hist: &mut[uint], sum: &mut[uint], hist_buckets: uint) -> uint
{
	assert_eq!(keys_in.len(), values_in.len());
	assert_eq!(keys_in.len(), keys_temp.len());
	assert_eq!(values_in.len(), values_temp.len());
	let hist_size = 1 << radix_bits;
	let key_hist_size = from_uint::<K>(hist_size).unwrap();
	let key_hist_mask = key_hist_size - one::<K>();
	let key_radix_bits = radix_bits;

	for key in keys_in.iter()
	{
		for bucket in range(0, hist_buckets)
		{
			let key_shift = bucket * radix_bits;
			let pos = ((*key >> key_shift) & key_hist_mask).to_uint().unwrap();
			let index = bucket * hist_size + pos;
			hist[index] += 1;
		}
	}

	sum_histograms(hist, sum, hist_buckets, hist_size);

	let mut key_bits = 0u;
	let mut i0 = 0;
	let mut i1 = hist_size;
	for i in range(0, hist_buckets)
	{
		let bucket = hist.mut_slice(i0, i1);
		match i & 1
		{
			0 => radix_pass(keys_in, keys_temp, values_in, values_temp, bucket,
					key_bits, key_hist_mask),
			_ => radix_pass(keys_temp, keys_in, values_temp, values_in, bucket,
					key_bits, key_hist_mask)
		}
		i0 += hist_size;
		i1 += hist_size;
		key_bits = key_bits + key_radix_bits;
	}
	return hist_buckets;
}



fn radix_sort_float<V: Clone>(radix_bits: uint,
	fkeys_in: &mut[f32], fkeys_temp: &mut[f32],
	values_in: &mut[V], values_temp: &mut[V],
	hist: &mut[uint], sum: &mut[uint], hist_buckets: uint) -> uint
{
	let keys_in: &mut[u32] = unsafe { transmute(fkeys_in) };
	let keys_temp: &mut[u32] = unsafe { transmute(fkeys_temp) };

	assert_eq!(keys_in.len(), values_in.len());
	assert_eq!(keys_in.len(), keys_temp.len());
	assert_eq!(values_in.len(), values_temp.len());

	let hist_size = 1 << radix_bits;
	let key_hist_size = from_uint::<u32>(hist_size).unwrap();
	let key_hist_mask = key_hist_size - 1;
	let key_radix_bits = radix_bits;

	for key_ref in keys_in.iter()
	{
		for bucket in range(0, hist_buckets)
		{
			let key = float_flip(*key_ref);
			let key_shift = bucket * radix_bits;
			let pos = ((key >> key_shift) & key_hist_mask) as uint;
			let index = bucket * hist_size + pos;
			hist[index] += 1;
		}
	}

	sum_histograms(hist, sum, hist_buckets, hist_size);

	let mut key_bits = 0u;
	let mut i0 = 0;
	let mut i1 = hist_size;

	{
		let bucket = hist.mut_slice(i0, i1);
		radix_pass_decode_float(keys_in, keys_temp, values_in, values_temp, bucket,
			key_bits, key_hist_mask);
		key_bits += key_radix_bits;
		i0 += hist_size;
		i1 += hist_size;
	}

	for i in range(1, hist_buckets - 1)
	{
		let bucket = hist.mut_slice(i0, i1);
		match i & 1
		{
			0 => radix_pass(keys_in, keys_temp, values_in, values_temp, bucket,
					key_bits, key_hist_mask),
			_ => radix_pass(keys_temp, keys_in, values_temp, values_in, bucket,
					key_bits, key_hist_mask)
		}
		key_bits += key_radix_bits;
		i0 += hist_size;
		i1 += hist_size;
	}

	{
		let bucket = hist.mut_slice(i0, i1);
		match (hist_buckets - 1) & 1
		{
			0 => radix_pass_encode_float(keys_in, keys_temp, values_in, values_temp, bucket,
					key_bits, key_hist_mask),
			_ => radix_pass_encode_float(keys_temp, keys_in, values_temp, values_in, bucket,
					key_bits, key_hist_mask)
		}
	}

	return hist_buckets;
}


pub fn radix8sort_u64<V: Clone>(
	keys_in: &mut[u64], keys_temp: &mut[u64],
	values_in: &mut[V], values_temp: &mut[V]) -> uint
{
	static kRadixBits: uint = 8;
	static kKeyBits: uint = 8 * 8;
	static kHistBuckets: uint = 1 + (kKeyBits - 1) / kRadixBits;
	static kHistSize: uint = 1 << kRadixBits;
	assert_eq!(kKeyBits, size_of::<u64>() * 8);

	let mut hist = [0u, ..kHistSize * kHistBuckets];
	let mut sum = [0u, ..kHistBuckets];

	return radix_sort_uint(kRadixBits, keys_in, keys_temp, values_in, values_temp,
		hist, sum, kHistBuckets);
}


pub fn radix8sort_u32<V: Clone>(
	keys_in: &mut[u32], keys_temp: &mut[u32],
	values_in: &mut[V], values_temp: &mut[V]) -> uint
{
	static kRadixBits: uint = 8;
	static kKeyBits: uint = 4 * 8;
	static kHistBuckets: uint = 1 + (kKeyBits - 1) / kRadixBits;
	static kHistSize: uint = 1 << kRadixBits;
	assert_eq!(kKeyBits, size_of::<f32>() * 8);
	assert_eq!(kKeyBits, size_of::<u32>() * 8);

	let mut hist = [0u, ..kHistSize * kHistBuckets];
	let mut sum = [0u, ..kHistBuckets];

	return radix_sort_uint(kRadixBits, keys_in, keys_temp, values_in, values_temp,
		hist, sum, kHistBuckets);
}


pub fn radix8sort_f32<V: Clone>(
	fkeys_in: &mut[f32], fkeys_temp: &mut[f32],
	values_in: &mut[V], values_temp: &mut[V]) -> uint
{
	static kRadixBits: uint = 8;
	static kKeyBits: uint = 4 * 8;
	static kHistBuckets: uint = 1 + (kKeyBits - 1) / kRadixBits;
	static kHistSize: uint = 1 << kRadixBits;
	assert_eq!(kKeyBits, size_of::<f32>() * 8);

	let mut hist = [0u, ..kHistSize * kHistBuckets];
	let mut sum = [0u, ..kHistBuckets];

	return radix_sort_float(kRadixBits, fkeys_in, fkeys_temp, values_in, values_temp,
		hist, sum, kHistBuckets);
}


pub fn radix11sort_u64<V: Clone>(
	keys_in: &mut[u64], keys_temp: &mut[u64],
	values_in: &mut[V], values_temp: &mut[V]) -> uint
{
	static kRadixBits: uint = 11;
	static kKeyBits: uint = 8 * 8;
	static kHistBuckets: uint = 1 + (kKeyBits - 1) / kRadixBits;
	static kHistSize: uint = 1 << kRadixBits;
	assert_eq!(kKeyBits, size_of::<u64>() * 8);

	let mut hist = [0u, ..kHistSize * kHistBuckets];
	let mut sum = [0u, ..kHistBuckets];

	return radix_sort_uint(kRadixBits, keys_in, keys_temp, values_in, values_temp,
		hist, sum, kHistBuckets);
}


pub fn radix11sort_u32<V: Clone>(
	keys_in: &mut[u32], keys_temp: &mut[u32],
	values_in: &mut[V], values_temp: &mut[V]) -> uint
{
	static kRadixBits: uint = 11;
	static kKeyBits: uint = 4 * 8;
	static kHistBuckets: uint = 1 + (kKeyBits - 1) / kRadixBits;
	static kHistSize: uint = 1 << kRadixBits;
	assert_eq!(kKeyBits, size_of::<u32>() * 8);

	let mut hist = [0u, ..kHistSize * kHistBuckets];
	let mut sum = [0u, ..kHistBuckets];

	return radix_sort_uint(kRadixBits, keys_in, keys_temp, values_in, values_temp,
		hist, sum, kHistBuckets);
}


pub fn radix11sort_f32<V: Clone>(
	fkeys_in: &mut[f32], fkeys_temp: &mut[f32],
	values_in: &mut[V], values_temp: &mut[V]) -> uint
{
	static kRadixBits: uint = 11;
	static kKeyBits: uint = 4 * 8;
	static kHistBuckets: uint = 1 + (kKeyBits - 1) / kRadixBits;
	static kHistSize: uint = 1 << kRadixBits;
	assert_eq!(kKeyBits, size_of::<f32>() * 8);

	let mut hist = [0u, ..kHistSize * kHistBuckets];
	let mut sum = [0u, ..kHistBuckets];

	return radix_sort_float(kRadixBits, fkeys_in, fkeys_temp, values_in, values_temp,
		hist, sum, kHistBuckets);
}

