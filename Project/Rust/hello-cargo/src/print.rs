pub fn run() {
    // print to console
    println!("Print from print.rs!");

    // Basic for formatting
    println!("{} is from {}.", "Tim", "China");

    // Position arguments
    println!("{0} is from {1} and {0} like {2}.  ", "Tim", "China", "Code");

    // Name arguments
    println!("{name} is like {activity}", name = "Tim", activity = "basketball");

    // Placeholder traits
    println!("Binary: {:b}, Hex: {:x}, Octal: {:o}", 10, 10, 10);

    // Placeholder for debug trait
    println!("{:?}", (12, true, "Yes"));

    // Basic math
    println!("10 + 50 = {}", 10 + 50);
} 