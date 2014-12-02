pub trait Zero {
	fn zero() -> Self;
}

impl Zero for u32 {
	fn zero() -> u32 { 0u32 }
}

impl Zero for u64 {
	fn zero() -> u64 { 0u64 }
}

impl Zero for f32 {
	fn zero() -> f32 { 0f32 }
}

pub fn check_sorted<K: PartialOrd, V: ToPrimitive>(
	keys: &[K], values: &[V], orig: &[K])
{
	assert_eq!(keys.len(), values.len());
	assert_eq!(keys.len(), orig.len());
	for i in range(1, keys.len())
	{
		if keys[i - 1] > keys[i]
		{
			println!("Key sort error at index {}", i - 1);
			assert!(false);
		}
		if keys[i] != orig[values[i].to_uint().unwrap()]
		{
			println!("Value sort error at index {}", i);
			assert!(false);
		}
	}
}

