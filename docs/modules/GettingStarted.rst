Getting Started
===============

What is this?
-------------

This a simulator and uses a combination of multiple physics packages to build a test environment for Unmanned Surface Vehicles (USV). 

Requirements
------------

- ubuntu 16.04
- ros-kinetic-desktop-full
- ros-kinetic-move-base
- libsdl-image1.2-dev
- libsdl-dev
- libfftw3-*
- libxml++2.6-*
- python-scipy
- python-geolinks
- python-gdal

Installation
------------

Go to the `Installation`_ page to install this test platform.

.. _`installation`: https://opencv-exercise.readthedocs.io/en/latest/modules/Installation.html

How to use
----------

1. roscore &
2. roslaunch usv_sim airboat_segmentation_2.launch parse:=true
3. roslaunch usv_sim airboat_segmentation_2.launch parse:=false
