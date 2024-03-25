# Lab 5 - Audio TX User Mode


See the lab instructions [here](usermode-hw-player\LabInstructions.md).


Data streams play an important role in multimedia embedded systems, such
as portable media players, set top boxes, and digital TVs. These devices
have often real-time requirements with regards to timely handling the
incoming and outgoing data streams. This lab will explore an audio
digital media stream. 

We will examine FIFO programming based on the AXI
Streaming FIFO of the Zynq SoC. A FIFO-based access is common also in
many other devices including protocol controllers, data converters and
hardware accelerators. Therefore, this portion of the lab gives you a
good overview of this interfacing type.

The audio setup is descripted [here](https://neu-ece-4534.github.io/audio.html).


## Overview 

The instructions for ths lab are detailed in the following steps:

 1. (Reserved for feedback branch pull request. You will receive top level feedback there). 
 2. [User-mode Player](.github/STARTING_ISSUES/2.%20User-mode%20Player.md)
 3. [Release 0.1 Documentation](.github/STARTING_ISSUES/3.%20Release%200.1%20Documentation.md)
 4. [Timed Polling](.github/STARTING_ISSUES/4.%20Timed%20Polling.md)
 5. [Release 0.2 Documentation](.github/STARTING_ISSUES/5.%20Release%200.2%20Documentation.md)


After accepting this assignment in github classroom, each step is converted into a [github issue](https://docs.github.com/en/issues). Follow the issues in numerically increasing issue number (the first issue is typically on the bottom of the list). 

## General Rules

Please commit your code frequently or at e very logical break. Each commit should have a meaningful commit message and [cross reference](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/autolinked-references-and-urls#issues-and-pull-requests) the issue the commit belongs to. Ideally, there would be no commits without referencing to a github issue. 

Please comment on each issue with the problems faced and your approach to solve them. Close an issue when done. 



