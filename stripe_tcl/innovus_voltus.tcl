#######################################################
#                                                     
#  Innovus Command Logging File                     
#  Created on Tue Feb  4 02:03:22 2020                
#                                                     
#######################################################

#@(#)CDS: Innovus v18.11-s100_1 (64bit) 09/17/2018 18:39 (Linux 2.6.18-194.el5)
#@(#)CDS: NanoRoute 18.11-s100_1 NR180_2819-2237/18_11-UB (database version 2.30, 428.7.1) {superthreading v1.46}
#@(#)CDS: AAE 18.11-s042 (64bit) 09/17/2018 (Linux 2.6.18-194.el5)
#@(#)CDS: CTE 18.11-s039_1 () Aug 15 2018 09:54:48 ( )
#@(#)CDS: SYNTECH 18.11-s016_1 () Aug 15 2018 09:49:01 ( )
#@(#)CDS: CPE v18.11-s099
#@(#)CDS: IQRC/TQRC 18.1.1-s552 (64bit) Sat May 19 16:19:10 PDT 2018 (Linux 2.6.18-194.el5)

set_global _enable_mmmc_by_default_flow      $CTE::mmmc_default
suppressMessage ENCEXT-2799
set init_gnd_net VSSX
set init_lef_file {/home1st/wilson27561/innovus/innovus_config/asap7_tech_4x_170803_withRC_4V12_OffsetAdj_newTzu_LISD_R.lef /home1st/wilson27561/innovus/innovus_config/lef/characterization_6T_ALL_20200610area_4x.lef}
set init_verilog /home1st/wilson27561/innovus/innovus_config/chip/neuralNetwork_V4_ASAP7_6t_fix_4pad.v
set_global timing_allow_input_delay_on_clock_source true
set init_mmmc_file /home1st/wilson27561/innovus/innovus_config/view/neuralNetwork_run0_6t.view
set init_pwr_net VDDX
init_design
setIoFlowFlag 0

floorPlan -site coreSite -r 0.8 0.7 10.0 10.0 10.0 10.0
saveIoFile -locations -temp /home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t32_run0_neuralNetwork.io
set dbgLefDefOutVersion 5.8
global dbgLefDefOutVersion
set dbgLefDefOutVersion 5.8
defOut -floorplan /home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t32_run0_neuralNetwork.def
reportGateCount

set infile [open "/home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t32_run0_neuralNetwork.def" r]
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

set infile [open "/home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t49run44_42_neuralNetwork.io" r]
set number 0
set numLeftPin 0
set numTopPin 0
set numRightPin 0
set numBottomPin 0
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
        set topPin($numTopPin) [lindex $lst4 1]
        incr numTopPin
        gets $infile line
        set indexp [string first "pin" $line]
        }
        if {$numTopPin > 1} {
        set topSidePins  $topPin(0)
        for {set i 1} {$i < $numTopPin} {incr i} {lappend topSidePins $topPin($i)}
        }
    }
    "right" {
        gets $infile line
        set indexp [string first "pin" $line]
        while {$indexp >= 0} {
        set lst4 [split $line "\""]
        set rightPin($numRightPin) [lindex $lst4 1]
        incr numRightPin
        gets $infile line
        set indexp [string first "pin" $line]
        }
        if {$numRightPin > 1} {
        set rightSidePins  $rightPin(0)
        for {set i 1} {$i < $numRightPin} {incr i} {lappend rightSidePins $rightPin($i)}
        }
    }
    "bottom" {
        gets $infile line
        set indexp [string first "pin" $line]
        while {$indexp >= 0} {
        set lst4 [split $line "\""]
        set bottomPin($numBottomPin) [lindex $lst4 1]
        incr numBottomPin
        gets $infile line
        set indexp [string first "pin" $line]
        }
        if {$numBottomPin > 1} {
        set bottomSidePins  $bottomPin(0)
        for {set i 1} {$i < $numBottomPin} {incr i} {lappend bottomSidePins $bottomPin($i)}
        }
    }
    "left" {
        gets $infile line
        set indexp [string first "pin" $line]
        while {$indexp >= 0} {
        set lst4 [split $line "\""]
        set leftPin($numLeftPin) [lindex $lst4 1]
        incr numLeftPin
        gets $infile line
        set indexp [string first "pin" $line]
        }
        if {$numLeftPin > 1} {
        set leftSidePins  $leftPin(0)
        for {set i 1} {$i < $numLeftPin} {incr i} {lappend leftSidePins $leftPin($i)}
        }
    }
}

}
puts $numTopPin
puts $numRightPin
puts $numBottomPin
puts $numLeftPin
close $infile

# set pinWidth 1.728
# set pinDepth 1.44
set pinWidth 0.072
set pinDepth 1.44
getPinAssignMode -pinEditInBatch -quiet
setPinAssignMode -pinEditInBatch true
# setting the coordinates must follow the clockwise direction
set editLeftSidePin "editPin -pinWidth $pinWidth -pinDepth $pinDepth -fixOverlap 1 -spreadDirection clockwise -side Left -layer M4 -spreadType range -start 0.0 $llyv -end 0.0 $uryv -pin"
lappend editLeftSidePin $leftSidePins
eval $editLeftSidePin
set editTopSidePin "editPin -pinWidth $pinWidth -pinDepth $pinDepth -fixOverlap 1 -spreadDirection clockwise -side Top -layer M3 -spreadType range -start $llxv $chipYdim -end $urxv $chipYdim -pin"
lappend editTopSidePin $topSidePins
eval $editTopSidePin
set editRightSidePin "editPin  -pinWidth $pinWidth -pinDepth $pinDepth -fixOverlap 1 -spreadDirection clockwise -side Right -layer M4 -spreadType range -start $chipXdim $uryv -end $chipXdim $llyv -pin"
lappend editRightSidePin $rightSidePins
eval $editRightSidePin
set editBottomSidePin "editPin -pinWidth $pinWidth -pinDepth $pinDepth -fixOverlap 1 -spreadDirection clockwise -side Bottom -layer M3 -spreadType range -start $urxv 0.0 -end $llxv 0.0 -pin"
lappend editBottomSidePin $bottomSidePins
eval $editBottomSidePin
set filePathAndName "/home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t49run.io"
puts $filePathAndName
saveIoFile -locations $filePathAndName 

globalNetConnect VDDX -type pgpin -pin VDD* -instanceBasename * -hierarchicalInstance {}
globalNetConnect VSSX -type pgpin -pin VSS* -instanceBasename * -hierarchicalInstance {}
globalNetConnect VDDX -type net -net VDD*
globalNetConnect VSSX -type net -net VSS*
set tLayer M7
set bLayer M1
set tLayerNo 6
set bLayerNo 2
set tLayerNameNo M7

#createPGPin VDD -net VDD
#createPGPin VSS -net VSS
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

source power_pad_stripe_6t49_neuralNetwork_routing_44_42_4pad.tcl

setMultiCpuUsage -localCpu 8 -cpuPerRemoteHost 1 -remoteHost 0 -keepLicense true
setDistributeHost -local
setRouteMode -earlyGlobalHonorMsvRouteConstraint false -earlyGlobalRoutePartitionPinGuide true
setEndCapMode -reset
setEndCapMode -boundary_tap false
setNanoRouteMode -quiet -droutePostRouteSpreadWire 1
setNanoRouteMode -quiet -timingEngine {}
setUsefulSkewMode -maxSkew false -noBoundary false -useCells {HB4xp67_ASAP7_6t_fix HB3xp67_ASAP7_6t_fix HB2xp67_ASAP7_6t_fix HB1xp67_ASAP7_6t_fix BUFx8_ASAP7_6t_fix BUFx6f_ASAP7_6t_fix BUFx5_ASAP7_6t_fix BUFx4f_ASAP7_6t_fix BUFx4_ASAP7_6t_fix BUFx3_ASAP7_6t_fix BUFx2_ASAP7_6t_fix BUFx24_ASAP7_6t_fix BUFx16f_ASAP7_6t_fix BUFx12f_ASAP7_6t_fix BUFx12_ASAP7_6t_fix BUFx10_ASAP7_6t_fix INVxp67_ASAP7_6t_fix INVxp33_ASAP7_6t_fix INVx8_ASAP7_6t_fix INVx6_ASAP7_6t_fix INVx5_ASAP7_6t_fix INVx4_ASAP7_6t_fix INVx3_ASAP7_6t_fix INVx2_ASAP7_6t_fix INVx1_ASAP7_6t_fix INVx13_ASAP7_6t_fix INVx11_ASAP7_6t_fix} -maxAllowedDelay 1


saveNetlist /home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/neuralNetwork_6t_4pad_before.v
reportGateCount
setAddStripeMode -ignore_block_check false -break_at none -route_over_rows_only false -rows_without_stripes_only false -extend_to_closest_target none -stop_at_last_wire_for_area false -partial_set_thru_domain false -ignore_nondefault_domains false -trim_antenna_back_to_shape none -spacing_type edge_to_edge -spacing_from_block 0 -stripe_min_length stripe_width -stacked_via_top_layer $tLayer -stacked_via_bottom_layer M1 -via_using_exact_crossover_size false -split_vias false -orthogonal_only true -allow_jog { padcore_ring  block_ring } -skip_via_on_pin {  standardcell } -skip_via_on_wire_shape {  noshape   }
setPlaceMode -fp false
setPlaceMode -checkCellDRCFromPreRoute false
place_design
saveDesign neuralNetwork_6t_4pad_run0.innovus


saveNetlist /home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/neuralNetwork_6t_4pad_after.v
reportGateCount
setSrouteMode -viaConnectToShape { noshape }
sroute -connect { blockPin padPin padRing corePin floatingStripe } -layerChangeRange "LISD $tLayerNameNo" -blockPinTarget { nearestTarget } -padPinPortConnect { allPort oneGeom } -padPinTarget { nearestTarget } -corePinTarget { firstAfterRowEnd } -floatingStripeTarget { blockring padring ring stripe ringpin blockpin followpin } -allowJogging 1 -corePinLayer {LISD M1 } -crossoverViaLayerRange "LISD $tLayerNameNo" -nets { VDDX VSSX } -allowLayerChange 0 -blockPin useLef -targetViaLayerRange "LISD $tLayerNameNo"
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
# setAddStripeMode -ignore_block_check false -break_at none -route_over_rows_only false -rows_without_stripes_only false -extend_to_closest_target none -stop_at_last_wire_for_area false -partial_set_thru_domain false -ignore_nondefault_domains false -trim_antenna_back_to_shape none -spacing_type edge_to_edge -spacing_from_block 0 -stripe_min_length stripe_width -stacked_via_top_layer $tLayer -stacked_via_bottom_layer M1 -via_using_exact_crossover_size false -split_vias false -orthogonal_only true -allow_jog { padcore_ring  block_ring } -skip_via_on_pin {  standardcell } -skip_via_on_wire_shape {  noshape   }
addStripe -nets {VDDX VSSX} -layer M3 -direction vertical -width 0.224 -spacing 0.072 -set_to_set_distance 3.77929 -start_from left -start_offset 0 -switch_layer_over_obs false -max_same_layer_jog_length 2 -padcore_ring_top_layer_limit $tLayer -padcore_ring_bottom_layer_limit M1 -block_ring_top_layer_limit $tLayer -block_ring_bottom_layer_limit M1 -use_wire_group 0 -snap_wire_center_to_grid None
setNanoRouteMode -quiet -routeBottomRoutingLayer  $bLayerNo
setNanoRouteMode -quiet -routeTopRoutingLayer $tLayerNo
ccopt_design
reportGateCount
refinePlace -checkRoute 0 -preserveRouting 0 -rmAffectedRouting 0 -swapEEQ 0
addFiller -cell FILLER_ASAP7_6t_fix FILLERxp5_ASAP7_6t_fix -prefix FILLER
checkplace
defOut /home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t32_MBuf_Asap7_tg_design_placed.def -routing -floorplan -netlist
saveNetlist /home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t32_neuralNetworkASAPplus_placed.v

setRouteMode -earlyGlobalHonorMsvRouteConstraint false -earlyGlobalRoutePartitionPinGuide true
setEndCapMode -reset
setEndCapMode -boundary_tap false
setNanoRouteMode -quiet -routeTopRoutingLayer $tLayerNo
setNanoRouteMode -quiet -routeWithTimingDriven true
setUsefulSkewMode -maxSkew false -noBoundary false -useCells {BUFx10_ASAP7_6t_fix BUFx12_ASAP7_6t_fix BUFx12f_ASAP7_6t_fix BUFx16f_ASAP7_6t_fix BUFx24_ASAP7_6t_fix BUFx2_ASAP7_6t_fix BUFx3_ASAP7_6t_fix BUFx4_ASAP7_6t_fix BUFx4f_ASAP7_6t_fix BUFx5_ASAP7_6t_fix BUFx6f_ASAP7_6t_fix BUFx8_ASAP7_6t_fix HB1xp67_ASAP7_6t_fix HB2xp67_ASAP7_6t_fix HB3xp67_ASAP7_6t_fix HB4xp67_ASAP7_6t_fix INVx11_ASAP7_6t_fix INVx13_ASAP7_6t_fix INVx1_ASAP7_6t_fix INVx2_ASAP7_6t_fix INVx3_ASAP7_6t_fix INVx4_ASAP7_6t_fix INVx5_ASAP7_6t_fix INVx6_ASAP7_6t_fix INVx8_ASAP7_6t_fix INVxp33_ASAP7_6t_fix INVxp67_ASAP7_6t_fix} -maxAllowedDelay 1
setNanoRouteMode -quiet -routeBottomRoutingLayer $bLayerNo
setNanoRouteMode -quiet -drouteEndIteration default
setNanoRouteMode -quiet -routeWithTimingDriven true
setNanoRouteMode -quiet -routeWithSiDriven false

#setNanoRouteMode -quiet -drouteOnGridOnly via
#The line below can be set to "false" to allow pin access using M1 wires
setNanoRouteMode -routeWithViaOnlyForStandardCellPin "true"
setNanoRouteMode -quiet -drouteSearchAndRepair true

routeDesign -globalDetail -viaOpt -wireOpt
setAnalysisMode -cppr none -clockGatingCheck true -timeBorrowing true -useOutputPinCap true -sequentialConstProp false -timingSelfLoopsNoSkew false -enableMultipleDriveNet true -clkSrcPath true -warn true -usefulSkew false -analysisType onChipVariation -log true
saveNetlist /home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/neuralNetwork_6t_4pad_after_routing_verySmall_run0.v
reportGateCount


setExtractRCMode -engine postRoute -effortLevel high -coupled false
reset_parasitics
extractRC

set spefName "/home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t49_neuralNetwork_routing_run0.spef"
rcOut -spef $spefName -rc_corner 6t32_neuralNetworkrun0_RCor
                                                                     
set designName "/home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t49_neuralNetwork_routing_run0.enc"
puts "Design Name = $designName"
saveDesign $designName

set defName "/home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/6t49_neuralNetwork_routing_run0.def"
puts "Def File Name = $defName"
defOut -floorplan -netlist -routing $defName

#repair
setRouteMode -earlyGlobalHonorMsvRouteConstraint false -earlyGlobalRoutePartitionPinGuide true
setEndCapMode -reset
setEndCapMode -boundary_tap false
setNanoRouteMode -quiet -drouteNoTaperOnOutputPin 1
setNanoRouteMode -quiet -drouteUseMultiCutViaEffort high
setUsefulSkewMode -maxSkew false -noBoundary false -useCells {BUFx10_ASAP7_6t_fix BUFx12_ASAP7_6t_fix BUFx12f_ASAP7_6t_fix BUFx16f_ASAP7_6t_fix BUFx24_ASAP7_6t_fix BUFx2_ASAP7_6t_fix BUFx3_ASAP7_6t_fix BUFx4_ASAP7_6t_fix BUFx4f_ASAP7_6t_fix BUFx5_ASAP7_6t_fix BUFx6f_ASAP7_6t_fix BUFx8_ASAP7_6t_fix HB1xp67_ASAP7_6t_fix HB2xp67_ASAP7_6t_fix HB3xp67_ASAP7_6t_fix HB4xp67_ASAP7_6t_fix INVx11_ASAP7_6t_fix INVx13_ASAP7_6t_fix INVx1_ASAP7_6t_fix INVx2_ASAP7_6t_fix INVx3_ASAP7_6t_fix INVx4_ASAP7_6t_fix INVx5_ASAP7_6t_fix INVx6_ASAP7_6t_fix INVx8_ASAP7_6t_fix INVxp33_ASAP7_6t_fix INVxp67_ASAP7_6t_fix} -maxAllowedDelay 1
setNanoRouteMode -quiet -drouteEndIteration default
setNanoRouteMode -quiet -routeWithTimingDriven true
setNanoRouteMode -quiet -routeWithSiDriven false

routeDesign -globalDetail -viaOpt -wireOpt
setAnalysisMode -cppr none -clockGatingCheck true -timeBorrowing true -useOutputPinCap true -sequentialConstProp false -timingSelfLoopsNoSkew false -enableMultipleDriveNet true -clkSrcPath true -warn true -usefulSkew false -analysisType onChipVariation -log true
saveNetlist /home1st/wilson27561/innovus/neuralNetwork/addStripe_6t_3/neuralNetwork_6t_4pad_after_repaire_nowireload_run0.v
reportGateCount

timeDesign -postRoute -pathReports -drvReports -slackReports -numPaths 5 -prefix neuralNetwork_postRoute -outDir timingReports