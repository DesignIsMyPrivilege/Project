// Conditionals - Used to check the condition of something and act on the result
pub fn run() {
    let age: u8 = 18;
    let check_id: bool = false;
    let knows_ones_age: bool = true;

    if age >= 18 && check_id || knows_ones_age {
        println!("Bartender: What would you like to drink?");
    } else if age < 18 && check_id {
        println!("Bartender: Sorry, you have to leave.");
    } else {
        println!("Bartender: I'll need to see your ID.");
    }

    // Shorthand If
    let is_of_age = if age >= 18 { true } else { false };
    println!("Is of age: {}", is_of_age);
}