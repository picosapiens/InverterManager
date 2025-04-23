# InverterManager
 Load detection and inverter activation/deactivation for efficient use of battery power.

This code runs on an STM32 Blue Pill (using the Roger Clark Melbourne core - not the official one) to activate an inverter upon detection of a closed circuit and then deactivate it after a timeout.

Electricity is very dangerous. Please don't try to wire this into mains power without consulting a qualified professional.