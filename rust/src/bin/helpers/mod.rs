use std::num::{ ToPrimitive };

pub fn check_sorted<K: PartialOrd, V: ToPrimitive>(
	keys: &[K], values: &[V], orig: &[K])
{
	assert_eq!(keys.len(), values.len());
	assert_eq!(keys.len(), orig.len());
	for i in (1..keys.len())
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

