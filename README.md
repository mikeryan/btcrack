
BTCrack OSS 1.1
===============
License : GPL3 see LICENSE
Authors : Eric Sesterhen & Thierry Zoller
Contact : http://www.cobra-basket.de & http://blog.zoller.lu
GitHub: : https://github.com/mikeryan/btcrack

Updates to Updates to 1.1 (by Mike Ryan)
========================================
+ Moved to GitHub
+ Added Makefile
+ Compile with -Wall -Werror
+ Minor bugfix

Updates to 1.1
==============
+ Format String bug (Michael Ossman)
+ Master ACO was overwritten by the the slave ACO thus impairing decryption (carl.dunhamm@hotmail.com)

Updates left to the reader (Courtesy  of Carl Dunhamm) : 
=======================================================
> line 446: It is not correct to always use bp->bdaddr_s for the in_bdaddr parameter of KeyInit. 
It is wrong to assume that INRAND always comes from the Master, sometimes it comes from the slave.
Indeed it depends whether the slave or the master actually sent the IN_RAND. If its the master then 
bdaddr_s has to be used, but if it's the slave then bdaddr_m has to be used.  

Suggested patch :
Detect whether inrand comes from Master or Slave and adjust the KeyInit call, alternatively
take appropriate parameter from the command-line to decide.

Details on the Patch of Carl Dunhamm
====================================
"GetSRES(bp->kab, bp->au_rand_s, bp->bdaddr_m, bp->calc_sres_m, bp->calc_aco_m);"
It shall be "bp->calc_aco_s" instead of "bp->calc_aco_m". 
With the orinigal version the master ACO is overwritten by the one of the slave.
This is not imparing the finding of the PIN or link key, but if master ACO has 
to be reused for some reason, one will get the wrong one when refering to calc_aco_m.

About
=====
This is a straight forward linux port of Thierry Zollers' BTCrack.
Should work with most other unixes too, code is nearly ansi clean, 
except for strdup(), but I guess every OS should have this by now.

Compiling was tested so far with
- gcc version 4.1.1 (Gentoo 4.1.1-r3) on i686-pc-linux-gnu
- gcc version 4.3.0-alpha20061216 on i586-pc-linux-gnu
- gcc version 3.3.6 on i586-pc-linux-gnu
- gcc version 3.4.6 on i586-pc-linux-gnu
- gcc version 2.95.4 20011002 (Debian prerelease) on i686-pc-linux-gnu
- gcc version 4.0.3 on sparc-sun-solaris2.8
- icc Version 9.1 Build 20060706Z on i686-pc-linux-gnu
- Sun WorkShop 6 update 2 C 5.3 Patch 111679-11 2003/04/02

Test it with the provided csv file:
./btcrack 1 00:11:9F:C4:F3:AE 00:60:57:1A:6B:F1 ./Pin_654321.csv

Contact
=======
You can contact Eric Sesterhen at http://www.cobra-basket.de or snakebyte@cccmz.de
You can contact Thierry Zoller at http://blog.zoller.lu

Comments 
========
Interestingly there is nearly no difference between icc and gcc on my 2400+ Sempron
box, the gcc seems slightly (~5%) faster. And only ~10% between -O3 and -Os. On my K6 the code
compiled with an mid-december gcc 4.3 snapshot is twice as fast as the 3.3.6 version :-)
On the solaris box, the sun compiler generated better code than the gcc (~15-20%)
The dual P3 box, gave me a 1 1/2 speedup of gcc 4.1 to gcc 2.95, 3.4 was only marginally
faster (~10%) than 2.95.
