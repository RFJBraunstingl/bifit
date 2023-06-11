cd $1
gradle build
unzip -od ../../jar build/libs/$1.jar
