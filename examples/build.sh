cd $1
gradle build
rm -rf ../../jar/*
unzip -od ../../jar build/libs/$1.jar
