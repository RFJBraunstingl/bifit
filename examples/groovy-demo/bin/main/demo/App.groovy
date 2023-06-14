package demo

class App {
    String getGreeting() {
        return 'Hello Groovy!'
    }

    static void main(String[] args) {
        def app = new App()
        println app.greeting
    }
}
