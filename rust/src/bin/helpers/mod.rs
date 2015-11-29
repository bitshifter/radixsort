#[macro_export]
macro_rules! check_sorted(
	($keys:ident, $values:ident, $orig: ident) =>
	{{
		assert_eq!($keys.len(), $values.len());
		assert_eq!($keys.len(), $orig.len());
		for i in (1..$keys.len())
		{
			if $keys[i - 1] > $keys[i]
			{
				println!("Key sort error at index {}", i - 1);
				assert!(false);
			}
			if $keys[i] != $orig[$values[i] as usize]
			{
				println!("Value sort error at index {}", i);
				assert!(false);
			}
		}
	}}
);

