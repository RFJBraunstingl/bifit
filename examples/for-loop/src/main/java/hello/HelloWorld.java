package hello;

public class HelloWorld {
    public static void main(String[] args) {
        Greeter greeter = new Greeter();
        for (;;) {
            greet(greeter);
        }
    }

    private static void greet(Greeter greeter) {
        System.out.println(greeter.sayHello());
    }
}
