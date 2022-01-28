// Variables hold primitive data or reference to data
// Variables are immutable by default
// Rust is a block-scoped language
pub fn run() {
    let name = "Tim";
    let mut age = "26";
    age = 18;
    println!("My name is {} and I am {}", name, age);

    // Define constant
    const ID: i32 = 001;
    println!("ID: {}", ID);

    // Assign multiple vars
    let ( my_name, my_age ) = ( "Tim", 18);
    println!("My name is {} and I am {}", my_name, my_age); 
}