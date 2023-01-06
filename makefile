all: subnetcalculator

subnetcalculator: SubnetCalculator.cpp
	g++ -o subnetcalculator SubnetCalculator.cpp

clean:
	rm subnetcalculator
