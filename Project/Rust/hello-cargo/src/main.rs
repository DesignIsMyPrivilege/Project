use std::collections::HashMap;
mod cli;
#[derive(Debug)]
struct TestStruct {
    number: u8,
    string: String,
    char: char,
}
#[derive(Debug)]
enum TestEnum {
    WEunit(bool),
    WEstruct(TestStruct),
    WEtuple(String, bool),
}

struct Person {
    first: String,
    middle: Option<String>,
    last: String,
}

fn tuple_test() {
    println!("Tuyple Test");
    let tuple_0 = ("The", 1u8, 'a', true);

    println!("Tuple value is {:?}", tuple_0);
}

fn array_test() {
    println!("Array Test");
    let array_0 = [1, 2, 3, 4];
    let array_1 = [1; 6];

    println!("The array_0 value is {:?}", array_0);
    println!("The array_1 value is {:?}", array_1);
}

fn struct_test() {
    let struct_0 = TestStruct{
        number: 10, 
        string: String::from("test"), 
        char: 'a'
    };

    println!("Struct value is {:?}", struct_0);
}

fn enum_test() {
    let we_unit = TestEnum::WEunit(true);
    let first_struct = TestStruct {
        number: 10, 
        string: String::from("test"), 
        char: 'a',
    };

    let we_struct = TestEnum::WEstruct(first_struct);
    let we_tuple = TestEnum::WEtuple(String::from("test"), true);

    println!("The we_unit value is {:?}, we_struct value is {:?}, we_tuple value is {:?}", we_unit, we_struct, we_tuple);
}

fn vec_test() {
    println!("Vec Test");
    let mut var_0 = Vec::new();
    var_0.push("The");
    var_0.push("Second");
    var_0.push("Third");

    println!("The var_0 value is {:?}", var_0);
    println!("The 4 values is {:?}", var_0.get(4));
}

fn hash_test() {
    println!("Hash Test");
    let mut hash_0: HashMap<String, String> = HashMap::new();
    hash_0.insert(String::from("The"), String::from("First"));

    println!("The hash_0 value is {:?}", hash_0);
}

fn panic_test() {
    panic!("Error Test");
}

fn option_test() {
    let var_0 = vec!["1", "2", "3", "4"];

    for &index in [0, 1, 5].iter() {
        match var_0.get(index) {
            Some(&"2") => println!("The 2 value!!"),
            Some(test) => println!("The test value is {}.", test),
            None => println!("There is no value! :("),
        }
    }
}

fn let_test() {
    let let_0: Option<u8> = Some(10); 

    if let Some(10) = let_0 {
        println!("Test True ! :)");
    }
}

fn build_full_name(person: &Person) -> String {
    let mut full_name = String::new();
    full_name.push_str(&person.first);
    full_name.push_str(" ");

    if let Some(middle) = &person.middle {
        full_name.push_str(&middle);
        full_name.push_str(" ");
    }

    full_name.push_str(&person.last);
    full_name
}

fn person_test() {
    let john = Person {
        first: String::from("James"),
        middle: Some(String::from("Oliver")),
        last: String::from("Smith"),
    };
    assert_eq!(build_full_name(&john), "James Oliver Smith");

    let alice = Person {
        first: String::from("Alice"),
        middle: None,
        last: String::from("Stevens"),
    };
    assert_eq!(build_full_name(&alice), "Alice Stevens");

    let bob = Person {
        first: String::from("Robert"),
        middle: Some(String::from("Murdock")),
        last: String::from("Jones"),
    };
    assert_eq!(build_full_name(&bob), "Robert Murdock Jones");
}

fn copy_and_return<'a>(vector: &'a mut Vec<String>, value: &'a str) -> &'a str {
    vector.push(String::from(value));

    value
}

fn live_test() {
    let name1 = "a";
    let name2 = "b";
    let name3 = "c";

    let mut names = Vec::new();

    assert_eq!("a", copy_and_return(&mut names, &name1));
    assert_eq!("b", copy_and_return(&mut names, &name2));
    assert_eq!("c", copy_and_return(&mut names, &name3));

    assert_eq!(names, vec!["a".to_string(), "b".to_string(), "c".to_string()]);

    println!("The name value is {:?}", names);
}

fn main() {
    println!("Hello, world!");
    cli::run();
    /*array_test();
    vec_test();
    hash_test();
    tuple_test();
    struct_test();
    enum_test();
    option_test();
    let_test();
    person_test();
    live_test();*/
}
