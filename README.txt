................................................................................
..M...MM..:M.MMMMM...MMMMM.....MMMMM..MMMMM....MMMM...MMMMM..M..MM....MM........
..M...MM..M..M......M..........M...MM.M . M..MM....M..M......M..M.M..M.M........
...M.M..M.M...MMMM..M..........MMMMM..MMMM...M.....MM. MMMM..M..M..MM..M........
...M.M..MM........M.M..........M......M . M..MM....M ......M.M..M..MM..M........
...MM...MM . MMMMM . MMMMM ....M......M .. M...MMMM...MMMMM .M..M..... M........
................................................................................
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
WSC Prosim 1.0.0
================

README for release 1.0.0 of 24-Sept-2009
========================================

This distribution contains the first release of the WSC's Prosim, processor scheduling and 
memory simulation software.

Copyright 2009 Wintermore Software Consultancy (WSC)
Jake Dallimore		dallij01@student.uwa.edu.au		SN: 20381063
Sam Winterton 		wintes02@student.uwa.edu.au		SN: 10508358

All of this product's content was created by Jake Dallimore & Sam Winterton.


DOCUMENTATION ROADMAP
=====================

This file contains the following sections:

1. OVERVIEW               General description of Prosim and the Prosim front-end GUI.
2. RELEASE NOTES	  What's new in the current release.
3. SYSTEM REQUIREMENTS    What you need to have in order to run Prosim.
4. INSTALLATION	          How to install Prosim.
5. GETTING STARTED	  How to set up and use Prosim.
6. KNOWN ISSUES/PROBLEMS  Any known issues with the behaviour of the current release.
7. VERSION HISTORY	  What has changed in the releases to date.  
8. PRICING		  Free!
9. CONTACT		  How to reach us if you have a query.
10.LEGAL & COPYRIGHT      Copyright, warranty & terms of distribution.


1. OVERVIEW
===========

Prosim is a processor scheduling and virtual memory simulation program, designed to mimic the 
behaviors of a simplistic real world operating system. It simulates both processor scheduling 
(cycle by cycle) and memory caching (physical and swap). Program output is in both graphical 
and textual forms.



2. RELEASE NOTES: RELEASE 1.0.0
===============================

First Release


3. SYSTEM REQUIREMENTS
======================

    3.1 HARDWARE REQUIREMENTS
    =========================

    Prosim will run on most machines, given software requirements are met.


    3.2 SOFTWARE REQUIREMENTS
    =========================

    The Prosim application can be accessed via a CLI or a GUI.
    All machines should be able to run the CLI version of the program. It has no dependencies except the c standard library.

    The GUI requires the following software libraries be installed:

    - Qt libraries 4.5. Also known as libqt4


    These can be found (for all architectures) at: 	 

    http://qt.nokia.com/downloads


4. INSTALLATION
===============
To build the CLI version of the software:

1.  Navigate to the prosimCLI directory via a CLI.
2.  Run the command "make clean".
3.  Run the command "make".
4.  Copy the "prosimmain" executable to the "prosimGUI/linux_binaries". (needed for the GUI to work correctly)
5.  Your distribution of Prosim is now ready to use.


5. GETTING STARTED
==================

To run the CLI version of the program, navigate to the "prosimCLI" directory and enter the command "./prosimmain"

	eg.	./prosimmain -h  					for usage
	or	./prosimmain -s -i test.file -a RR -q 1			for scheduling mode 
	or	./prosimmain -v -i test.file -a RR -q 2 -x 10		for virtual memory mode

To run the GUI version of the program, navigate to the "prosimGUI/linux_binaries" directory and either enter the command "./prosim" or click the "prosim" executable


6. KNOWN ISSUES/ PROBLEMS
=========================

- Minor scaling bugs exist
- Process run time display in the GUI is limited to 1000 units of time
- GUI will exit if the file does not exist and run is clicked


7. VERSION HISTORY
==================

0.89 Sam drank a coke and needed pain medication
0.90 The development team attended the seminar "Introduction to the c programming language"
0.91 No version history exists yet.



8. PRICING
==========

Prosim is freeware and is provided for distribution at no cost.


9. CONTACT US AT
================

Wintermore Software Consultancy

Jake Dallimore
dallij01@student.uwa.edu.au (distribution requests, administration)

Sam Winterton
wintes02@student.uwa.edu.au (distribution requests, administration)

support@wmsc.thisisafakeaddress.com (bugs, problems, questions)

800/555-1212 (M-F, 9 AM-4 PM WST)


10. LEGAL & COPYRIGHT
=====================





