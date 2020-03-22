============
Installation
============

Docker (Recommended)
====================

coming soon...


via Github
==========

Creating a catkin workspace

.. code-block:: bash

    source /opt/ros/kinetic/setup.bash
    mkdir -p ~/catkin_ws/src
    cd ~/catkin_ws/
    catkin_make

Clone the usv_sim repository in the src folder of your catkin workspace:

.. code-block:: bash

    cd ~/catkin_ws/src
    git clone https://github.com/star-cuhksz/STAr.git
    cd ~/catkin_ws
    catkin_make_isolated --install
    source ~/usv_sim_lsa/install_isolated/setup.bash
