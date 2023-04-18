#Package with pip

all:
	rm -f Build/MARTe
	ln -s $(PWD)/Source Build/MARTe
	pip install --upgrade .

clean:
	rm -f Build/MARTe
	pip uninstall MARTe
