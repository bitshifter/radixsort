use std::mem;
use std::num::Int;
use std::ptr;

const THRESHOLD: int = 16;

fn lg(n: uint) -> uint {
	mem::size_of::<uint>() * 8 - 1 - n.leading_zeros()
}

fn ptr_diff<T>(a: *const T, b: *const T) -> int {
    let ai = a as uint;
    let bi = b as uint;
    let d =  if ai < bi { bi - ai } else { ai - bi } / mem::size_of::<T>();
    d as int
}

fn insertion_sort<T, F>(first: *mut T, last: *mut T, lt: &F) where F: Fn(&T, &T) -> bool {
    let len = ptr_diff(first as *const T, last as *const T);

    // 1 <= i < len;
    for i in range(1, len) {
        // j satisfies: 0 <= j <= i;
        let mut j = i;
        unsafe {
            // `i` is in bounds.
            let read_ptr = first.offset(i) as *const T;

            // find where to insert, we need to do strict <,
            // rather than <=, to maintain stability.

            // 0 <= j - 1 < len, so .offset(j - 1) is in bounds.
            while j > 0 &&
                    lt(&*read_ptr, &*first.offset(j - 1)) {
                j -= 1;
            }

            // shift everything to the right, to make space to
            // insert this value.

            // j + 1 could be `len` (for the last `i`), but in
            // that case, `i == j` so we don't copy. The
            // `.offset(j)` is always in bounds.

            if i != j {
                let tmp = ptr::read(read_ptr);
                ptr::copy_memory(first.offset(j + 1),
                                 &*first.offset(j),
                                 (i - j) as uint);
                ptr::copy_nonoverlapping_memory(first.offset(j),
                                                &tmp as *const T,
                                                1);
                mem::forget(tmp);
            }
        }
    }
}

fn median<T, F>(v: &[T], a: uint, b: uint, c: uint, lt: F) -> uint where F: Fn(&T, &T) -> bool {
    if lt(&v[a], &v[b]) {
        if lt(&v[b], &v[c]) {
            b
        }
        else if lt(&v[a], &v[c]) {
            c
        }
        else {
            a
        }
    }
    else if lt(&v[a], &v[c]) {
        a
    }
    else if lt(&v[b], &v[c]) {
        c
    }
    else {
        b
    }
}

fn median_3<T, F>(a: *mut T, b: *mut T, c: *mut T, lt: &F) -> *mut T where F: Fn(&T, &T) -> bool {
    unsafe {
        if lt(&*a, &*b) {
            if lt(&*b, &*c) {
                b
            }
            else if lt(&*a, &*c) {
                c
            }
            else {
                a
            }
        }
        else if lt(&*a, &*c) {
            a
        }
        else if lt(&*b, &*c) {
            c
        }
        else {
            b
        }
    }
}

fn partition<T, F>(mut first: *mut T, mut last: *mut T, pivot: *mut T, lt: &F) -> *mut T
        where F: Fn(&T, &T) -> bool {
    unsafe {
        loop {
            while lt(&*first, &*pivot) {
                first = first.offset(1);
            }
            last = last.offset(-1);
            while lt(&*pivot, &*last) {
                last = last.offset(-1);
            }
            if !((first as uint) < (last as uint)) {
                return first;
            }
            ptr::swap(first, last);
            first = first.offset(1);
        }
    }
}

fn partition_pivot<T, F>(first: *mut T, last: *mut T, lt: &F) -> *mut T
        where F: Fn(&T, &T) -> bool {
    unsafe {
        let len = ptr_diff(first as *const T, last as *const T);
        let pivot = median_3(first.offset(1), first.offset(len / 2), first.offset(len - 1), lt);
        ptr::swap(first, pivot);
        partition(first.offset(1), last, first, lt)
    }
}

fn intro_sort_loop<T, F>(first: *mut T, mut last: *mut T, mut depth_limit: uint, lt: &F) where F: Fn(&T, &T) -> bool {
    while ptr_diff(last as *const T, first as *const T) > THRESHOLD {
        if depth_limit == 0 {
            insertion_sort(first, last, lt);
            return;
        }
        depth_limit -= 1;
        let pivot = partition_pivot(first, last, lt);
        intro_sort_loop(pivot, last, depth_limit, lt);
        last = pivot;
    }
}

pub fn sort_by<T: PartialOrd, F>(v: &mut[T], lt: F) where F: Fn(&T, &T) -> bool {
    let len = v.len();
    if len > 0 {
        unsafe {
            let first = v.as_mut_ptr();
            let last = first.offset(len as int - 1);
            intro_sort_loop(first, last, 2 * lg(len), &lt);
            insertion_sort(first, last, &lt);
        }
    }
}

pub fn sort<T: PartialOrd>(v: &mut[T]) {
    sort_by(v, |a, b| a.le(b))
}
