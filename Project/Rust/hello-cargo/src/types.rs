/*
 Primitive Types
    Integers: u8, i8, u16, i16, u32, i32, u64, i64, u128, i128 (number of bits they take in memory)
    Floats: f32, f64
    Boolean (bool)
    Characters (char)
    Tuples
    Arrays
*/

// Rust is statically typed language, which means that is must know the types of all variables at compile time.
// however, the compiler can usually infer what type we want to use based on the value and how to use it.
pub fn run() {
    // Default is "i32".
    let x = 1;
    
    // Default is "f64".
    let y = 2.3;
    
    // Add explicit type
    let z: i64 = 58585858888;

    // Find max size
    println!("i32 Max: {}", std::i32::MAX);
    println!("i64 Max: {}", std::i64::MAX);

    // Boolean
    let is_active: bool = true;

    // Get boolean from expression
    let is_greater: bool = 10 > 5;

    // Char type
    let a1 = 'a';
    let face = '\u{1F600}';

    println!("{:?}", (x, y, z, is_active, is_greater, a1, face)); 
}