source "helpers.tcl"
read_lef Nangate45/Nangate45.lef
read_liberty Nangate45/Nangate45_typ.lib
read_def est_rc2.def

global_route -layers 2-10 \
  		  -unidirectional_routing
estimate_parasitics -global_routing

report_net -connections -verbose -digits 3 clk
report_net -connections -verbose -digits 3 u2/ZN
