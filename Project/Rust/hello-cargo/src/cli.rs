// CLI
use std::env;

pub fn run() {
    let args: Vec<String> = env::args().collect();
    let command = args[1].clone();
    let name = "Tim";
    let status = "100%";

    if command == "hello" {
        println!("helle {}, how are you?", name);
    } else if command == "status" {
        println!("Status is {}.", status);
    } else {
        println!("That is not a valid command!");
    }
}