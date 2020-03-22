====================================================
Welcome to USV Test Platform's documentation!
====================================================

USV Test Platform uses a test environment called `usv_sim_lsa`_.
It is developed to test control and trajectory strategies for USVs, but it can be easily adapted to other applications.
It contains multiple robot models such as propelled boats(rudder boat, differential boat, airboat) and sailboat. 
Boats are affected by waves, wind and water currents, implemented by UWsim for water surface modeling, HEC-RAS for water speed of river and channel simulations, and Lattice Boltzmann in a 2D grid for wind current.
All those features allow to modelling the movement of boats in a realistic way.

.. _`usv_sim_lsa`: https://github.com/disaster-robotics-proalertas/usv_sim_lsa

.. toctree::
   :hidden:
   :glob:

   modules/GattingStarted
   modules/Installation
   modules/Customization
   modules/LeaderBoard
