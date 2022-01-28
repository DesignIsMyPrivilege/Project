// Loops - Used to iterate until a condition is met
pub fn run() {
    let mut count = 0;

    // Infinite loop
    // loop {
    //     count += 1;
    //     println!("Number: {}", count);

    //     if count == 20 {
    //         break;
    //     }
    // }

    // while loop (FizzBuzz)
    // while count <= 20 {
    //     if count % 15 == 0 {
    //         println!("FizzBuss");
    //     } else if count % 3 == 0 {
    //         println!("Fizz");
    //     } else if count % 5 == 0 {
    //         println!("Buss");
    //     } else {
    //         println!("{}", count);
    //     }
    //     count += 1;
    // }

    // For Ranges
    for x in 0..100 {
        if x % 15 == 0 {
            println!("FizzBuss");
        } else if x % 3 == 0 {
            println!("Fizz");
        } else if x % 5 == 0 {
            println!("Buss");
        } else {
            println!("{}", x);
        }
    }
    
}