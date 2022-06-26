#
#
#
# proc offsetGTZero { topSidePins oldTopSidePins leftSidePins oldLeftSidePins  bottomSidePins oldBottomSidePins rightSidePins oldRightSidePins } {
#     set minx [llength $topSidePins]
#     for { set inx 0 } { $inx < $minx } { incr inx } {
#    	    set netName [lindex $topSidePins $inx]
# 	    if { [string first "VDD" $netName] != 0 && [string first "VSS" $netName] != 0 } {
#                set cmdSelectNet "selectNet { "
# 	       append cmdSelectNet $netName " }"
# 	       puts "Top $inx $cmdSelectNet"
# 	       eval $cmdSelectNet
#             }
#     }
#     set minx [llength $leftSidePins]
#     for {set inx 0} { $inx < $minx } {incr inx} {
#    	    set netName [lindex $leftSidePins $inx]
# 	    if { [string first "VDD" $netName] != 0 && [string first "VSS" $netName] != 0 } {
#                set cmdSelectNet "selectNet { "
# 	       append cmdSelectNet  $netName " }"
# 	       puts "Left $inx $cmdSelectNet"
# 	       eval $cmdSelectNet
#             }
#     }
#     set minx [llength $bottomSidePins]
#     for {set inx 0} { $inx < $minx } {incr inx} {
#    	    set netName [lindex $bottomSidePins $inx]
# 	    if { [string first "VDD" $netName] != 0 && [string first "VSS" $netName] != 0 } {
#                set cmdSelectNet "selectNet { "
# 	       append cmdSelectNet  $netName " }"
# 	       puts "Bottom $inx $cmdSelectNet"
# 	       eval $cmdSelectNet
#             }
#     }
#     set minx [llength $rightSidePins]
#     for {set inx 0} { $inx < $minx } {incr inx} {
#    	    set netName [lindex $rightSidePins $inx]
# 	    if { [string first "VDD" $netName] != 0 && [string first "VSS" $netName] != 0 } {
#                set cmdSelectNet "selectNet { "
# 	       append cmdSelectNet  $netName " }"
# 	       puts "Right $inx $cmdSelectNet"
# 	       eval $cmdSelectNet
#             }
#     }

#     selectNet { VSSX }
#     selectNet { VDDX }
#     setNanoRouteMode -quiet -routeSelectedNetOnly 1
#     setNanoRouteMode -quiet -drouteEndIteration default
#     setNanoRouteMode -quiet -routeWithTimingDriven true
#     setNanoRouteMode -quiet -routeWithSiDriven false
#     routeDesign -globalDetail
#     deselectAll
# }

# proc offsetZero {} {
# globalNetConnect VDDX -type pgpin -pin VDD* -instanceBasename * -hierarchicalInstance {}
# globalNetConnect VSSX -type pgpin -pin VSS* -instanceBasename * -hierarchicalInstance {}
# globalNetConnect VDDX -type net -net VDD*
# globalNetConnect VSSX -type net -net VSS*
# set tLayer M7
# set bLayer M1
# set tLayerNo 6
# set bLayerNo 2
# set tLayerNameNo M7
# # createPGPin VDD -net VDD
# # createPGPin VSS -net VSS
# set sprCreateIeRingOffset 1.0
# set sprCreateIeRingThreshold 1.0
# set sprCreateIeRingJogDistance 1.0
# set sprCreateIeRingLayers {}
# set sprCreateIeRingOffset 1.0
# set sprCreateIeRingThreshold 1.0
# set sprCreateIeRingJogDistance 1.0
# set sprCreateIeRingLayers {}
# set sprCreateIeStripeWidth 10.0
# set sprCreateIeStripeThreshold 1.0
# set sprCreateIeStripeWidth 10.0
# set sprCreateIeStripeThreshold 1.0
# set sprCreateIeRingOffset 1.0
# set sprCreateIeRingThreshold 1.0
# set sprCreateIeRingJogDistance 1.0
# set sprCreateIeRingLayers {}
# set sprCreateIeStripeWidth 10.0
# set sprCreateIeStripeThreshold 1.0
# setAddRingMode -ring_target default -extend_over_row 0 -ignore_rows 0 -avoid_short 0 -skip_crossing_trunks none -stacked_via_top_layer $tLayer -stacked_via_bottom_layer M1 -via_using_exact_crossover_size 1 -orthogonal_only true -skip_via_on_pin {  standardcell } -skip_via_on_wire_shape {  noshape }
# addRing -nets {VDDX VSSX} -type core_rings -follow core -layer {top M2 bottom M2 left M1 right M1} -width {top 1.8 bottom 1.8 left 1.8 right 1.8} -spacing {top 0.072 bottom 0.072 left 0.072 right 0.072} -offset {top 1.8 bottom 1.8 left 1.8 right 1.8} -center 0 -extend_corner {} -threshold 0 -jog_distance 0 -snap_wire_center_to_grid None
# addWellTap -cell TAPCELL_ASAP7_6t_fix -cellInterval 25.92 -inRowOffset 12.89 -prefix WELLTAP
# setMultiCpuUsage -localCpu 8 -cpuPerRemoteHost 1 -remoteHost 0 -keepLicense true
# setDistributeHost -local
# setRouteMode -earlyGlobalHonorMsvRouteConstraint false -earlyGlobalRoutePartitionPinGuide true
# setEndCapMode -reset
# setEndCapMode -boundary_tap false
# setNanoRouteMode -quiet -droutePostRouteSpreadWire 1
# setNanoRouteMode -quiet -timingEngine {}
# setUsefulSkewMode -maxSkew false -noBoundary false -useCells {HB4xp67_ASAP7_6t_fix HB3xp67_ASAP7_6t_fix HB2xp67_ASAP7_6t_fix HB1xp67_ASAP7_6t_fix BUFx8_ASAP7_6t_fix BUFx6f_ASAP7_6t_fix BUFx5_ASAP7_6t_fix BUFx4f_ASAP7_6t_fix BUFx4_ASAP7_6t_fix BUFx3_ASAP7_6t_fix BUFx2_ASAP7_6t_fix BUFx24_ASAP7_6t_fix BUFx16f_ASAP7_6t_fix BUFx12f_ASAP7_6t_fix BUFx12_ASAP7_6t_fix BUFx10_ASAP7_6t_fix INVxp67_ASAP7_6t_fix INVxp33_ASAP7_6t_fix INVx8_ASAP7_6t_fix INVx6_ASAP7_6t_fix INVx5_ASAP7_6t_fix INVx4_ASAP7_6t_fix INVx3_ASAP7_6t_fix INVx2_ASAP7_6t_fix INVx1_ASAP7_6t_fix INVx13_ASAP7_6t_fix INVx11_ASAP7_6t_fix} -maxAllowedDelay 1
# setPlaceMode -fp false
# setPlaceMode -checkCellDRCFromPreRoute false
# # setAddStripeMode -ignore_block_check false -break_at none -route_over_rows_only false -rows_without_stripes_only false -extend_to_closest_target none -stop_at_last_wire_for_area false -partial_set_thru_domain false -ignore_nondefault_domains false -trim_antenna_back_to_shape none -spacing_type edge_to_edge -spacing_from_block 0 -stripe_min_length stripe_width -stacked_via_top_layer $tLayer -stacked_via_bottom_layer M1 -via_using_exact_crossover_size false -split_vias false -orthogonal_only true -allow_jog { padcore_ring  block_ring } -skip_via_on_pin {  standardcell } -skip_via_on_wire_shape {  noshape   }
# # addStripe -nets { VDDX } -layer M3 -direction vertical -width 0.112 -spacing 0.072 -set_to_set_distance 12.88 -start_from left -start_offset 12.88 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# # addStripe -nets { VSSX } -layer M3 -direction vertical -width 0.112 -spacing 0.072 -set_to_set_distance 12.88 -start_from left -start_offset 6.44 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# # source add_stripe_PD1_6t49_b19_routing_88_avg_1.tcl
# place_design
# setSrouteMode -viaConnectToShape { noshape }
# set sprCreateIeRingOffset 1.0
# set sprCreateIeRingThreshold 1.0
# set sprCreateIeRingJogDistance 1.0
# set sprCreateIeRingLayers {}
# set sprCreateIeRingOffset 1.0
# set sprCreateIeRingThreshold 1.0
# set sprCreateIeRingJogDistance 1.0
# set sprCreateIeRingLayers {}
# set sprCreateIeStripeWidth 10.0
# set sprCreateIeStripeThreshold 1.0
# set sprCreateIeStripeWidth 10.0
# set sprCreateIeStripeThreshold 1.0
# set sprCreateIeRingOffset 1.0
# set sprCreateIeRingThreshold 1.0
# set sprCreateIeRingJogDistance 1.0
# set sprCreateIeRingLayers {}
# set sprCreateIeStripeWidth 10.0
# set sprCreateIeStripeThreshold 1.0
# sroute -connect { blockPin padPin padRing corePin floatingStripe } -layerChangeRange "LISD $tLayerNameNo" -blockPinTarget { nearestTarget } -padPinPortConnect { allPort oneGeom } -padPinTarget { nearestTarget } -corePinTarget { firstAfterRowEnd } -floatingStripeTarget { blockring padring ring stripe ringpin blockpin followpin } -allowJogging 1 -corePinLayer {LISD M1 } -crossoverViaLayerRange "LISD $tLayerNameNo" -nets { VDDX VSSX } -allowLayerChange 0 -blockPin useLef -targetViaLayerRange "LISD $tLayerNameNo"
# setAddStripeMode -ignore_block_check false -break_at none -route_over_rows_only false -rows_without_stripes_only false -extend_to_closest_target none -stop_at_last_wire_for_area false -partial_set_thru_domain false -ignore_nondefault_domains false -trim_antenna_back_to_shape none -spacing_type edge_to_edge -spacing_from_block 0 -stripe_min_length stripe_width -stacked_via_top_layer $tLayer -stacked_via_bottom_layer M1 -via_using_exact_crossover_size false -split_vias false -orthogonal_only true -allow_jog { padcore_ring  block_ring } -skip_via_on_pin {  standardcell } -skip_via_on_wire_shape {  noshape   }
# # addStripe -nets { VDDX } -layer M3 -direction vertical -width 0.216 -spacing 0.072 -set_to_set_distance 12.88 -start_from left -start_offset 12.88 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# # addStripe -nets { VSSX } -layer M3 -direction vertical -width 0.112 -spacing 0.072 -set_to_set_distance 12.88 -start_from left -start_offset 6.44 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# addStripe -nets {VDDX VSSX} -layer M3 -direction vertical -width 0.224 -spacing 0.072 -set_to_set_distance 2.48702 -start_from left -start_offset 0 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# setNanoRouteMode -quiet -routeBottomRoutingLayer  $bLayerNo
# setNanoRouteMode -quiet -routeTopRoutingLayer $tLayerNo
# ccopt_design
# refinePlace -checkRoute 0 -preserveRouting 0 -rmAffectedRouting 0 -swapEEQ 0
# addFiller -cell FILLER_ASAP7_6t_fix FILLERxp5_ASAP7_6t_fix -prefix FILLER
# checkplace
# defOut /home1st/wilson27561/innovus/b17/addStripe_6t_61/6t32_MBuf_Asap7_tg_design_placed.def -routing -floorplan -netlist
# saveNetlist /home1st/wilson27561/innovus/b17/addStripe_6t_61/6t32_b19ASAPplus_placed.v

# setRouteMode -earlyGlobalHonorMsvRouteConstraint false -earlyGlobalRoutePartitionPinGuide true
# setEndCapMode -reset
# setEndCapMode -boundary_tap false
# setNanoRouteMode -quiet -routeTopRoutingLayer $tLayerNo
# setNanoRouteMode -quiet -routeWithTimingDriven true
# setUsefulSkewMode -maxSkew false -noBoundary false -useCells {BUFx10_ASAP7_6t_fix BUFx12_ASAP7_6t_fix BUFx12f_ASAP7_6t_fix BUFx16f_ASAP7_6t_fix BUFx24_ASAP7_6t_fix BUFx2_ASAP7_6t_fix BUFx3_ASAP7_6t_fix BUFx4_ASAP7_6t_fix BUFx4f_ASAP7_6t_fix BUFx5_ASAP7_6t_fix BUFx6f_ASAP7_6t_fix BUFx8_ASAP7_6t_fix HB1xp67_ASAP7_6t_fix HB2xp67_ASAP7_6t_fix HB3xp67_ASAP7_6t_fix HB4xp67_ASAP7_6t_fix INVx11_ASAP7_6t_fix INVx13_ASAP7_6t_fix INVx1_ASAP7_6t_fix INVx2_ASAP7_6t_fix INVx3_ASAP7_6t_fix INVx4_ASAP7_6t_fix INVx5_ASAP7_6t_fix INVx6_ASAP7_6t_fix INVx8_ASAP7_6t_fix INVxp33_ASAP7_6t_fix INVxp67_ASAP7_6t_fix} -maxAllowedDelay 1
# setNanoRouteMode -quiet -routeBottomRoutingLayer $bLayerNo
# setNanoRouteMode -quiet -drouteEndIteration default
# setNanoRouteMode -quiet -routeWithTimingDriven true
# setNanoRouteMode -quiet -routeWithSiDriven false
# #setNanoRouteMode -quiet -drouteOnGridOnly via
# #The line below can be set to "false" to allow pin access using M1 wires
# #setNanoRouteMode -quiet -routeWithViaInPin true

# setNanoRouteMode -routeWithViaOnlyForStandardCellPin "true"
# setNanoRouteMode -quiet -drouteSearchAndRepair true
# routeDesign -globalDetail -viaOpt -wireOpt
# } 

######################################################################################################

set_global _enable_mmmc_by_default_flow      $CTE::mmmc_default
suppressMessage ENCEXT-2799
set init_gnd_net VSSX
set init_lef_file {/home1st/wilson27561/innovus/innovus_config/asap7_tech_4x_170803_withRC_4V12_OffsetAdj_newTzu_LISD_R.lef /home1st/wilson27561/innovus/innovus_config/lef/characterization_6T_ALL_20200610area_4x.lef}
set init_verilog /home1st/wilson27561/innovus/innovus_config/chip/b19_6t_1pad.v
#set init_verilog /home1st/wilson27561/innovus/b17/addStripe_6t_61/b19_LISD_fourpp_all.v
#set init_verilog /home1st/wilson27561/innovus/b17/addStripe_6t_61/b19_LISD_threepp.v
set init_mmmc_file /home1st/wilson27561/innovus/innovus_config/view/b19run0_6t.view
set init_pwr_net VDDX
init_design
setIoFlowFlag 0

floorPlan -site coreSite -r 0.8 0.7 10.0 10.0 10.0 10.0
saveIoFile -locations -temp /home1st/wilson27561/innovus/b17/addStripe_6t_61/6t32_run0_b19.io 
global dbgLefDefOutVersion
set dbgLefDefOutVersion 5.8
defOut -floorplan /home1st/wilson27561/innovus/b17/addStripe_6t_61/6t32_run0_b19.def

set infile [open "/home1st/wilson27561/innovus/b17/addStripe_6t_61/6t32_run0_b19.def" r]
set number 0
while { $number != 6 } {
gets $infile line
set lst1 [split $line " "]
set keyw [lindex $lst1 5]
set firstToken [lindex $lst1 0]
if {[string compare "UNITS" $firstToken] == 0} {
set dbUnits [lindex $lst1 3]
incr number
}
if {[string compare "DIEAREA" $firstToken] == 0} {
set xdim [lindex $lst1 6]
set ydim [lindex $lst1 7]
set chipXdim [expr {double($xdim)/double($dbUnits)}]
set chipYdim [expr {double($ydim)/double($dbUnits)}]
puts $chipXdim
puts $chipYdim
incr number
}

switch $keyw {
    "FE_CORE_BOX_LL_X" {
        set  llxv [lindex $lst1 7]
        incr number
    }
    "FE_CORE_BOX_LL_Y" {
        set  llyv [lindex $lst1 7]
        incr number
    }
    "FE_CORE_BOX_UR_X" {
        set  urxv [lindex $lst1 7]
        incr number
    }
    "FE_CORE_BOX_UR_Y" {
        set  uryv [lindex $lst1 7]
        incr number
    }
}

}
puts $llxv
puts $llyv
puts $urxv
puts $uryv
close $infile

set infile [open "/home1st/wilson27561/innovus/b17/addStripe_6t_61/6t32_run0_b19.io" r]
set number 0
set numLeftPin 0
set numTopPin 0
set numRightPin 0
set numBottomPin 0
set numNonPGpin 0
set numVDD 0
set numVSS 0
while {[gets $infile line] >= 0} {
set lst11 [split $line "("]
set lst2 [lindex $lst11 1]
set lst3 [split $lst2 " "]
set side [lindex $lst3 0]
switch $side {
    "top" {
        gets $infile line
        set indexp [string first "pin" $line]
        puts $indexp
        while {$indexp >= 0} {
        set lst4 [split $line "\""]
	set pinName [lindex $lst4 1]
	set subStr [string range $pinName 0 2]
	if {[string match "VDD" $subStr] == 1} {
        set VDDAry($numVDD) $pinName
	incr numVDD
	} else {
	   if {[string match "VSS" $subStr] == 1} {
	   set VSSAry($numVSS) $pinName
           incr numVSS
	   } else {
	      set nonPGpinAry($numNonPGpin) $pinName
 	      incr numNonPGpin
	  }
	}
        gets $infile line
	puts $line
        set indexp [string first "pin" $line]
        }
    }
    "right" {
        gets $infile line
        set indexp [string first "pin" $line]
        while {$indexp >= 0} {
        set lst4 [split $line "\""]
	set pinName [lindex $lst4 1]
	set subStr [string range $pinName 0 2]
	if {[string match "VDD" $subStr] == 1} {
        set VDDAry($numVDD) $pinName
	incr numVDD
	} else {
	   if {[string match "VSS" $subStr] == 1} {
	   set VSSAry($numVSS) $pinName
           incr numVSS
	   } else {
	      set nonPGpinAry($numNonPGpin) $pinName
 	      incr numNonPGpin
	  }
	}
        gets $infile line
        set indexp [string first "pin" $line]
        }
    }
    "bottom" {
        gets $infile line
        set indexp [string first "pin" $line]
        while {$indexp >= 0} {
        set lst4 [split $line "\""]
	set pinName [lindex $lst4 1]
	set subStr [string range $pinName 0 2]
	if {[string match "VDD" $subStr] == 1} {
        set VDDAry($numVDD) $pinName
	incr numVDD
	} else {
	   if {[string match "VSS" $subStr] == 1} {
	   set VSSAry($numVSS) $pinName
           incr numVSS
	   } else {
	      set nonPGpinAry($numNonPGpin) $pinName
 	      incr numNonPGpin
	  }
	}
        gets $infile line
        set indexp [string first "pin" $line]
        }
    }
    "left" {
        gets $infile line
        set indexp [string first "pin" $line]
        while {$indexp >= 0} {
        set lst4 [split $line "\""]
	set pinName [lindex $lst4 1]
	set subStr [string range $pinName 0 2]
	if {[string match "VDD" $subStr] == 1} {
        set VDDAry($numVDD) $pinName
	incr numVDD
	} else {
	   if {[string match "VSS" $subStr] == 1} {
	   set VSSAry($numVSS) $pinName
           incr numVSS
	   } else {
	      set nonPGpinAry($numNonPGpin) $pinName
 	      incr numNonPGpin
	  }
	}
        gets $infile line
        set indexp [string first "pin" $line]
        } ;#end of while
    }
} ;#end of switch
} ;#end of while
close $infile
# Calculate the number of pins per side
set totalNumPin [expr {$numNonPGpin + $numVDD + $numVSS}]
set remainder [expr {$totalNumPin % 4}]
set quotient [expr {$totalNumPin / 4}]
set numTopPin $quotient
set numRightPin $quotient
set numBottomPin $quotient
set numLeftPin $quotient
if { $remainder == 3 } {
  incr numTopPin
  incr numLeftPin
  incr numBottomPin
} elseif { $remainder == 2 } {
  incr numTopPin
  incr numLeftPin
} elseif { $remainder == 1 } {
  incr numTopPin
}
# Evenly spread the VDD pins around chip's periferal
set numPinPerSectVDD [expr {$numNonPGpin / $numVDD} ]  
# original 改掉這邊指定放置數量
# set numPinPerSectVDD 3
# The value of offsetPos can be from 0 to $numPinPerSectVDD. it is used to shift the position of the first VDD pin
# This loop is set to generate more than io files for different offsets of VDD pins withe respect to the top-left
# corener clockwise. When offsetPos is 0, the VDD pin on the top side is placed at the left most postion. If it is $numPinPerSectVDD, 
# then it is placed at $numPinPerSectVDD to the right of the top left corner clockwise. 
# initOffset specifies the initial offset position of first VDD with respect to the left-most end of top edge
# step specifies the increase of offsetPos per run of IO placement. It should be at lease one and smaller than $numPinPerSectVDD
set initOffset 0
set step 1

for {set offsetPos $initOffset } {$offsetPos <= $numPinPerSectVDD } {set offsetPos [expr $offsetPos + $step] } {
set sectNum 0
set j 0
for {set i 0} {$i <= $numNonPGpin} {incr i} {
    if {$i == [expr {[expr {$sectNum}] * $numPinPerSectVDD + $offsetPos}] && $sectNum < $numVDD} {
	set tempAry($j) $VDDAry($sectNum)
	incr sectNum
	incr j
    puts "i= $i"
    }
    if {$i < $numNonPGpin} {
      set tempAry($j) $nonPGpinAry($i)
      incr j
    }
}
# puts "j= $j i= $i sectNum= $sectNum "
set numNonPGPlusVDDpin [expr {$numNonPGpin + $numVDD}]
set numPinPerSectVSS [expr {$numNonPGPlusVDDpin / $numVSS}]  ;# already consider the increase of VDD pins
# Here, offssetPos for VSS can have its own value to be independent of the offsetPos for VDD. For the time being,
# the offsetPos for VSS is set equal to the offsetPos for VDD. If it is set independently, its value can be
# from 0 to $numPinPerSectVSS
set sectNum 0
set j 0
for {set i 0} {$i <= $numNonPGPlusVDDpin} {incr i} {
    if {$i == [expr {[expr {$sectNum}] * $numPinPerSectVSS + $offsetPos}] && $sectNum < $numVSS } {
        set allPinAry($j) $VSSAry($sectNum)
        incr sectNum
	incr j
    }
    if { $i < $numNonPGPlusVDDpin } {
      set allPinAry($j) $tempAry($i)
      incr j
    }
}
set topLeftSum [expr {$numTopPin + $numLeftPin}]
set topLeftBottomSum [expr {$numTopPin + $numLeftPin + $numBottomPin}]
set topSidePins  $allPinAry(0)
for {set i 1} {$i < $totalNumPin} {incr i} {
    if {$i < $numTopPin} {
	lappend topSidePins $allPinAry($i)
    } elseif { $i == $numTopPin } {
	set leftSidePins $allPinAry($i)
    } elseif { $i < $topLeftSum } {
	lappend leftSidePins $allPinAry($i)
    } elseif { $i == $topLeftSum } {
    	set bottomSidePins $allPinAry($i)
    } elseif { $i < $topLeftBottomSum } {
	lappend bottomSidePins $allPinAry($i)
    } elseif { $i == $topLeftBottomSum } {
	set rightSidePins $allPinAry($i)
    } else { lappend rightSidePins $allPinAry($i) }
}

set tempList $leftSidePins
set leftSidePins $rightSidePins
set rightSidePins $tempList
lreverse $leftSidePins
lreverse $rightSidePins

puts "None Power/Ground pins:"
for {set j 0} {$j < $numNonPGpin} {incr j} {
puts $nonPGpinAry($j)
}
puts "Power pins: "
for {set k 0} {$k < $numVDD} {incr k} {puts $VDDAry($k)}
puts "Ground pins: "
for {set m 0} {$m < $numVSS} {incr m} {puts $VSSAry($m)}
puts "Total number of pins: $totalNumPin"
puts "Number of top-side pins: $numTopPin"
puts "Number of right-side pins: $numRightPin"
puts "Number of bottom-side pins: $numBottomPin"
puts "Number of left-side pins: $numLeftPin"
puts "Number of power pins: $numVDD"
puts "Number of ground pins: $numVSS"
puts "NumPerSectVDD $numPinPerSectVDD"
puts "NumPerSectVSS $numPinPerSectVSS"
puts "Offset $offsetPos"
set pinWidth 1.728
set pinDepth 1.44
getPinAssignMode -pinEditInBatch -quiet
setPinAssignMode -pinEditInBatch true
# setting the coordinates must follow the clockwise direction
set editLeftSidePin "editPin -pinWidth $pinWidth -pinDepth $pinDepth -fixOverlap 1 -spreadDirection clockwise -side Left -layer M3 -spreadType range -start 0.0 $llyv -end 0.0 $uryv -pin"
lappend editLeftSidePin $leftSidePins
eval $editLeftSidePin
set editTopSidePin "editPin -pinWidth $pinWidth -pinDepth $pinDepth -fixOverlap 1 -spreadDirection clockwise -side Top -layer M4 -spreadType range -start $llxv $chipYdim -end $urxv $chipYdim -pin"
lappend editTopSidePin $topSidePins
eval $editTopSidePin
set editRightSidePin "editPin  -pinWidth $pinWidth -pinDepth $pinDepth -fixOverlap 1 -spreadDirection clockwise -side Right -layer M3 -spreadType range -start $chipXdim $uryv -end $chipXdim $llyv -pin"
lappend editRightSidePin $rightSidePins
eval $editRightSidePin
set editBottomSidePin "editPin -pinWidth $pinWidth -pinDepth $pinDepth -fixOverlap 1 -spreadDirection clockwise -side Bottom -layer M4 -spreadType range -start $urxv 0.0 -end $llxv 0.0 -pin"
lappend editBottomSidePin $bottomSidePins
eval $editBottomSidePin
set filePathAndName "/home1st/wilson27561/innovus/b17/addStripe_6t_61/6t49run"
append filePathAndName $numVDD $numVSS "_" $offsetPos ".io"
puts $filePathAndName
saveIoFile -locations $filePathAndName 
# Do the complete placement and route first time
if { $offsetPos == 9 || $offsetPos == 29 || $offsetPos == 49 || $offsetPos == 68} { 
globalNetConnect VDDX -type pgpin -pin VDD* -instanceBasename * -hierarchicalInstance {}
globalNetConnect VSSX -type pgpin -pin VSS* -instanceBasename * -hierarchicalInstance {}
globalNetConnect VDDX -type net -net VDD*
globalNetConnect VSSX -type net -net VSS*
set tLayer M7
set bLayer M1
set tLayerNo 6
set bLayerNo 2
set tLayerNameNo M7
# createPGPin VDD -net VDD
# createPGPin VSS -net VSS
set sprCreateIeRingOffset 1.0
set sprCreateIeRingThreshold 1.0
set sprCreateIeRingJogDistance 1.0
set sprCreateIeRingLayers {}
set sprCreateIeRingOffset 1.0
set sprCreateIeRingThreshold 1.0
set sprCreateIeRingJogDistance 1.0
set sprCreateIeRingLayers {}
set sprCreateIeStripeWidth 10.0
set sprCreateIeStripeThreshold 1.0
set sprCreateIeStripeWidth 10.0
set sprCreateIeStripeThreshold 1.0
set sprCreateIeRingOffset 1.0
set sprCreateIeRingThreshold 1.0
set sprCreateIeRingJogDistance 1.0
set sprCreateIeRingLayers {}
set sprCreateIeStripeWidth 10.0
set sprCreateIeStripeThreshold 1.0
setAddRingMode -ring_target default -extend_over_row 0 -ignore_rows 0 -avoid_short 0 -skip_crossing_trunks none -stacked_via_top_layer $tLayer -stacked_via_bottom_layer M1 -via_using_exact_crossover_size 1 -orthogonal_only true -skip_via_on_pin {  standardcell } -skip_via_on_wire_shape {  noshape }
addRing -nets {VDDX VSSX} -type core_rings -follow core -layer {top M2 bottom M2 left M1 right M1} -width {top 1.8 bottom 1.8 left 1.8 right 1.8} -spacing {top 0.072 bottom 0.072 left 0.072 right 0.072} -offset {top 1.8 bottom 1.8 left 1.8 right 1.8} -center 0 -extend_corner {} -threshold 0 -jog_distance 0 -snap_wire_center_to_grid None
addWellTap -cell TAPCELL_ASAP7_6t_fix -cellInterval 25.92 -inRowOffset 12.89 -prefix WELLTAP

set defName "/home1st/wilson27561/innovus/b17/addStripe_6t_61/6t49_b19_routing_"
append defName  $numVDD $numVSS "_" $offsetPos ".def" 
puts "Def File Name = $defName"
defOut -floorplan -netlist -routing $defName

# source power_pad_stripe_tcl_58.tcl
# setMultiCpuUsage -localCpu 8 -cpuPerRemoteHost 1 -remoteHost 0 -keepLicense true
# setDistributeHost -local
# setRouteMode -earlyGlobalHonorMsvRouteConstraint false -earlyGlobalRoutePartitionPinGuide true
# setEndCapMode -reset
# setEndCapMode -boundary_tap false
# setNanoRouteMode -quiet -droutePostRouteSpreadWire 1
# setNanoRouteMode -quiet -timingEngine {}
# setUsefulSkewMode -maxSkew false -noBoundary false -useCells {HB4xp67_ASAP7_6t_fix HB3xp67_ASAP7_6t_fix HB2xp67_ASAP7_6t_fix HB1xp67_ASAP7_6t_fix BUFx8_ASAP7_6t_fix BUFx6f_ASAP7_6t_fix BUFx5_ASAP7_6t_fix BUFx4f_ASAP7_6t_fix BUFx4_ASAP7_6t_fix BUFx3_ASAP7_6t_fix BUFx2_ASAP7_6t_fix BUFx24_ASAP7_6t_fix BUFx16f_ASAP7_6t_fix BUFx12f_ASAP7_6t_fix BUFx12_ASAP7_6t_fix BUFx10_ASAP7_6t_fix INVxp67_ASAP7_6t_fix INVxp33_ASAP7_6t_fix INVx8_ASAP7_6t_fix INVx6_ASAP7_6t_fix INVx5_ASAP7_6t_fix INVx4_ASAP7_6t_fix INVx3_ASAP7_6t_fix INVx2_ASAP7_6t_fix INVx1_ASAP7_6t_fix INVx13_ASAP7_6t_fix INVx11_ASAP7_6t_fix} -maxAllowedDelay 1
# setPlaceMode -fp false
# setPlaceMode -checkCellDRCFromPreRoute false
# # setAddStripeMode -ignore_block_check false -break_at none -route_over_rows_only false -rows_without_stripes_only false -extend_to_closest_target none -stop_at_last_wire_for_area false -partial_set_thru_domain false -ignore_nondefault_domains false -trim_antenna_back_to_shape none -spacing_type edge_to_edge -spacing_from_block 0 -stripe_min_length stripe_width -stacked_via_top_layer $tLayer -stacked_via_bottom_layer M1 -via_using_exact_crossover_size false -split_vias false -orthogonal_only true -allow_jog { padcore_ring  block_ring } -skip_via_on_pin {  standardcell } -skip_via_on_wire_shape {  noshape   }
# # addStripe -nets { VDDX } -layer M3 -direction vertical -width 0.112 -spacing 0.072 -set_to_set_distance 12.88 -start_from left -start_offset 12.88 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# # addStripe -nets { VSSX } -layer M3 -direction vertical -width 0.112 -spacing 0.072 -set_to_set_distance 12.88 -start_from left -start_offset 6.44 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# # source add_stripe_PD1_6t49_b19_routing_88_avg_1.tcl
# place_design
# setSrouteMode -viaConnectToShape { noshape }
# set sprCreateIeRingOffset 1.0
# set sprCreateIeRingThreshold 1.0
# set sprCreateIeRingJogDistance 1.0
# set sprCreateIeRingLayers {}
# set sprCreateIeRingOffset 1.0
# set sprCreateIeRingThreshold 1.0
# set sprCreateIeRingJogDistance 1.0
# set sprCreateIeRingLayers {}
# set sprCreateIeStripeWidth 10.0
# set sprCreateIeStripeThreshold 1.0
# set sprCreateIeStripeWidth 10.0
# set sprCreateIeStripeThreshold 1.0
# set sprCreateIeRingOffset 1.0
# set sprCreateIeRingThreshold 1.0
# set sprCreateIeRingJogDistance 1.0
# set sprCreateIeRingLayers {}
# set sprCreateIeStripeWidth 10.0
# set sprCreateIeStripeThreshold 1.0
# sroute -connect { blockPin padPin padRing corePin floatingStripe } -layerChangeRange "LISD $tLayerNameNo" -blockPinTarget { nearestTarget } -padPinPortConnect { allPort oneGeom } -padPinTarget { nearestTarget } -corePinTarget { firstAfterRowEnd } -floatingStripeTarget { blockring padring ring stripe ringpin blockpin followpin } -allowJogging 1 -corePinLayer {LISD M1 } -crossoverViaLayerRange "LISD $tLayerNameNo" -nets { VDDX VSSX } -allowLayerChange 0 -blockPin useLef -targetViaLayerRange "LISD $tLayerNameNo"
# setAddStripeMode -ignore_block_check false -break_at none -route_over_rows_only false -rows_without_stripes_only false -extend_to_closest_target none -stop_at_last_wire_for_area false -partial_set_thru_domain false -ignore_nondefault_domains false -trim_antenna_back_to_shape none -spacing_type edge_to_edge -spacing_from_block 0 -stripe_min_length stripe_width -stacked_via_top_layer $tLayer -stacked_via_bottom_layer M1 -via_using_exact_crossover_size false -split_vias false -orthogonal_only true -allow_jog { padcore_ring  block_ring } -skip_via_on_pin {  standardcell } -skip_via_on_wire_shape {  noshape   }
# # addStripe -nets { VDDX } -layer M3 -direction vertical -width 0.216 -spacing 0.072 -set_to_set_distance 12.88 -start_from left -start_offset 12.88 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# # addStripe -nets { VSSX } -layer M3 -direction vertical -width 0.112 -spacing 0.072 -set_to_set_distance 12.88 -start_from left -start_offset 6.44 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# addStripe -nets {VDDX VSSX} -layer M3 -direction vertical -width 0.224 -spacing 0.072 -set_to_set_distance 3.77929 -start_from left -start_offset 0 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
# # source add_Stripe_tcl_39_36stripes.tcl
# setNanoRouteMode -quiet -routeBottomRoutingLayer  $bLayerNo
# setNanoRouteMode -quiet -routeTopRoutingLayer $tLayerNo
# ccopt_design
# refinePlace -checkRoute 0 -preserveRouting 0 -rmAffectedRouting 0 -swapEEQ 0
# addFiller -cell FILLER_ASAP7_6t_fix FILLERxp5_ASAP7_6t_fix -prefix FILLER
# checkplace
# defOut /home1st/wilson27561/innovus/b17/addStripe_6t_61/6t32_MBuf_Asap7_tg_design_placed.def -routing -floorplan -netlist
# saveNetlist /home1st/wilson27561/innovus/b17/addStripe_6t_61/6t32_b19ASAPplus_placed.v

# setRouteMode -earlyGlobalHonorMsvRouteConstraint false -earlyGlobalRoutePartitionPinGuide true
# setEndCapMode -reset
# setEndCapMode -boundary_tap false
# setNanoRouteMode -quiet -routeTopRoutingLayer $tLayerNo
# setNanoRouteMode -quiet -routeWithTimingDriven true
# setUsefulSkewMode -maxSkew false -noBoundary false -useCells {BUFx10_ASAP7_6t_fix BUFx12_ASAP7_6t_fix BUFx12f_ASAP7_6t_fix BUFx16f_ASAP7_6t_fix BUFx24_ASAP7_6t_fix BUFx2_ASAP7_6t_fix BUFx3_ASAP7_6t_fix BUFx4_ASAP7_6t_fix BUFx4f_ASAP7_6t_fix BUFx5_ASAP7_6t_fix BUFx6f_ASAP7_6t_fix BUFx8_ASAP7_6t_fix HB1xp67_ASAP7_6t_fix HB2xp67_ASAP7_6t_fix HB3xp67_ASAP7_6t_fix HB4xp67_ASAP7_6t_fix INVx11_ASAP7_6t_fix INVx13_ASAP7_6t_fix INVx1_ASAP7_6t_fix INVx2_ASAP7_6t_fix INVx3_ASAP7_6t_fix INVx4_ASAP7_6t_fix INVx5_ASAP7_6t_fix INVx6_ASAP7_6t_fix INVx8_ASAP7_6t_fix INVxp33_ASAP7_6t_fix INVxp67_ASAP7_6t_fix} -maxAllowedDelay 1
# setNanoRouteMode -quiet -routeBottomRoutingLayer $bLayerNo
# setNanoRouteMode -quiet -drouteEndIteration default
# setNanoRouteMode -quiet -routeWithTimingDriven true
# setNanoRouteMode -quiet -routeWithSiDriven false
# #setNanoRouteMode -quiet -drouteOnGridOnly via
# #The line below can be set to "false" to allow pin access using M1 wires
# #setNanoRouteMode -quiet -routeWithViaInPin true

# setNanoRouteMode -routeWithViaOnlyForStandardCellPin "true"
# setNanoRouteMode -quiet -drouteSearchAndRepair true
# routeDesign -globalDetail -viaOpt -wireOpt
# set oldLeftSidePins $leftSidePins
# set oldRightSidePins $rightSidePins
# set oldTopSidePins $topSidePins
# set oldBottomSidePins $bottomSidePins

# #setAnalysisMode -cppr none -clockGatingCheck true -timeBorrowing true -useOutputPinCap true -sequentialConstProp false -timingSelfLoopsNoSkew false -enableMultipleDriveNet true -clkSrcPath true -warn true -usefulSkew false -analysisType onChipVariation -log true

# #timeDesign -postRoute -pathReports -drvReports -slackReports -numPaths 5 -prefix b19_postRoute -outDir timingReports
# setExtractRCMode -engine postRoute -effortLevel high -coupled false
# reset_parasitics
# extractRC
# set spefName "/home1st/wilson27561/innovus/b17/addStripe_6t_61/6t49_b19_routing_"
# append spefName  $numVDD $numVSS "_" $offsetPos ".spef"
# rcOut -spef $spefName -rc_corner 6t32_b19run0_RCor
                                                                     
# set designName "/home1st/wilson27561/innovus/b17/addStripe_6t_61/6t49_b19_routing_"
# append designName  $numVDD $numVSS "_" $offsetPos ".enc" 
# puts "Design Name = $designName"
# saveDesign $designName
# set defName "/home1st/wilson27561/innovus/b17/addStripe_6t_61/6t49_b19_routing_"
# append defName  $numVDD $numVSS "_" $offsetPos ".def" 
# puts "Def File Name = $defName"
# defOut -floorplan -netlist -routing $defName
    } 
} 

