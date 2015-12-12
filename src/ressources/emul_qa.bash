#!/bin/bash

###########################################################
#                                                         #
# Author : Thomas Morgand                                 #
# e-mail : thomasm@eve-team.com                           #
# Date   : 10/28/08                                       #
#                                                         #
###########################################################
#                                                         #
# Purpose : Try to launch the Makefiel as qa_launch does. #
#           The script uses test.config file and an User's#
#           Environment file.                             #
#                                                         #
# Version : 0.0                                           #
#                                                         #
###########################################################

# test.config not present
if [ ! -e "test.config" ]
then
  echo ""
  echo "  test.config File not present !!!"
  echo ""
  exit 1 
fi

FILE_CONF=`echo -n ${FILE_CONF:-notset}`
if [ $FILE_CONF == "notset" ]
then
  echo ""
  echo "  FILE_CONF variable not set !!!"
  echo ""
  exit 1 
fi

# Check the number of arguments
if [ $# -lt 1 ]
then
  echo ""
  echo "  Usage: $0 < Makefile Target > <Optional STIMULU> <Optional TG_COMP> <Optional TG_RUN>"
  echo "        - Optional STIMULU: launch only this Stimulu"
  echo "        - Optional TG_COMP: launch only this TG_COMP"
  echo "        - Optional TG_RUN : launch only this TG_RUN"
  echo ""
  echo "  Note: Makefile Target are:"
  echo "        clean"
  echo "        compil"
  echo "        compil_gui"
  echo "        run"
  echo ""
  exit 1
else

  makefile_target=$1

  if [ $# -gt 1 ]
  then
    stimulu_to_use=$2
    if [ ! -d ${stimulu_to_use} ]
    then
      echo ""
      echo "  ${stimulu_to_use} directory does not exist !!!"
      echo ""
      exit 1
    fi
  else
    stimulu_to_use="none"
  fi

  if [ $# -gt 2 ]
  then
    tg_comp_to_use=$3
    if [ ` grep $tg_comp_to_use ./test.config | grep -v "^ *#" | wc -l` -eq 0 ]
    then
      echo ""
      echo "  $tg_comp_to_use is not a valid target"
      echo ""
      exit 1
    fi
  else
    tg_comp_to_use="none"
  fi
  if [ $# -eq 4 ]
  then
    tg_run_to_use=$4
  else
    tg_run_to_use=`cat test.config | grep "|" | sed "s/\*//g" | grep ${tg_comp_to_use} | grep "^ *$stimulu_to_use|\|^ *$stimulu_to_use  *|" | sed "s/\*//g" | cut -d\| -f3 | sed "s/ //g" | sed "s/\t//g" | head -1` 
    if [ -t $tg_run_to_use ]
    then
      tg_run_to_use="none"
    fi
  fi
fi

# User gives too much arguments
if [ $# -gt 4 ]
then
  echo ""
  echo "  Warning: Be carefull you provide more arguments than necessary."
  echo "           Some arguments will not be used."
  echo ""
fi

############################################
# Set PC_RUN variable
export PC_RUN=$HOSTNAME

############################################
# Call QA scripts to create QA environment 

if [ ! -f compil.env ]
then

  /auto/qa/scripts/qa_config_parsing.pl $FILE_CONF
  
  echo "
export PC_RUN=$HOSTNAME
export FILE_CONF=$FILE_CONF
export ZEBU_SYSTEM_DIR=$ZEBU_SYSTEM_DIR
export ZEBU_ROOT=$ZEBU_ROOT
export ZEBU_IP_ROOT=$ZEBU_IP_ROOT" >> compil.env
  if [ -n "${ZEBU_XIL:+x}" ]
  then
    echo "
export ZEBU_XIL=$ZEBU_XIL" >> compil.env
  else
    echo "
export ZEBU_XIL=/usr/import/xilinx/ise13.4_patched/ISE_DS/" >> compil.env
  fi
  echo "
export ZEBU_DRIVER_PATH=$ZEBU_ROOT/drivers
export PATH=$ZEBU_ROOT/bin:$PATH
export LIBRARY_PATH=$ZEBU_ROOT/lib:$ZEBU_ROOT/lib32
export TCL_LIBRARY=$ZEBU_ROOT/tcl/tcl8.4
export TCL_LIBRARY_PATH=$ZEBU_ROOT/tcl:$ZEBU_ROOT/tcl32
export LD_LIBRARY_PATH=$LIBRARY_PATH:$TCL_LIBRARY_PATH$EXTRACT_PATH_LIB
export REMOTECMD=\"$REMOTECMD\"
export SYNTH_TOOLS=$SYNTH_TOOLS
export REMOTESYN=\"$REMOTESYN\"" >> compil.env
  
  source compil.env
else
  echo ""    
  echo -en '\E[10;32m' "\033[1m  compil.env file not sourced !!!\033[0m"
  echo ""    
  echo ""    
fi
############################################
# Launch only 1 Config into Stimulu given by User,
# and using TG_COMP and TG_RUN given by User

if [ $# -gt 2 ]
then

  export TG_COMP=$tg_comp_to_use
  export TG_RUN=$tg_run_to_use
  
  # Check if Compilation step is allowed
  if [ $TG_COMP == "NTE" ] && [ $makefile_target == "compil"]
  then
    echo ""
    echo "  TG_COMP is equal to NTE so no Compil. can be performed."
    echo ""
    exit 0
  fi

  if [ $TG_COMP == "NTE" ] && [ $makefile_target == "compil_gui" ]
  then
    echo ""
    echo "  TG_COMP is equal to NTE so no Compil. can be performed."
    echo ""
    exit 0
  fi
  
  # Check if Run step is allowed
  if [ $TG_RUN == "NTE" ] && [ $makefile_target == "run" ]
  then
    echo ""
    echo "  TG_RUN is equal to NTE so no Run can be performed."
    echo ""
    exit 0
  fi 
  
  # Compil Part
  if [ $makefile_target == "compil" ] || [ $makefile_target == "compil_gui" ]
  then
    t0=`date +%s`
    make -C $stimulu_to_use $makefile_target TG_COMP=$TG_COMP
    if [ $? -ne 0 ]
    then
      echo "" 
      echo "---------------------------- Report emul_qa.bash ----------------------------"
      echo "  Test FAILED"
    else
      echo "" 
      echo "---------------------------- Report emul_qa.bash ----------------------------"
      echo "  Test OK"
    fi 
    t1=`date +%s`
    echo "" 
    echo "  Duration time (seconds) = `expr $t1 - $t0`";
    echo "" 
    exit 0
  fi
  
  # RunTime part
  if [ $# -gt 3 ]
  then
    t0=`date +%s`
    make -C $stimulu_to_use $makefile_target
    if [ $? -ne 0 ]
    then
      echo "" 
      echo "---------------------------- Report emul_qa.bash ----------------------------"
      echo "  Test FAILED"
    else
      echo "" 
      echo "---------------------------- Report emul_qa.bash ----------------------------"
      echo "  Test OK"
    fi 
    t1=`date +%s`
    echo "" 
    echo "  Duration time (seconds) = `expr $t1 - $t0`";
    echo "" 
    exit 0
  fi
fi

############################################
# Launch all Stimuli with all TG_COMP and all TG_RUN
# or Launch a Stimulu with all TG_COMP and all TG_RUN 

# Determine the number of available stimuli
if [ $stimulu_to_use == "none" ]
then
  max_line=`grep "|" ./test.config | grep -v "^ *#" | wc -l | sed "s/  */ /" | sed "s/^ *//" | cut -d" " -f1`
else                               
  if [ $# -eq 3 ]
  then
    max_line=`grep "|" ./test.config | grep -v "^ *#" | grep "^ *$stimulu_to_use|\|^ *$stimulu_to_use  *|" | grep -w $tg_comp_to_use | wc -l | sed "s/  */ /" | sed "s/^ *//" | cut -d" " -f1`
  else
    max_line=`grep "|" ./test.config | grep -v "^ *#" | grep "^ *$stimulu_to_use|\|^ *$stimulu_to_use  *|" | wc -l | sed "s/  */ /" | sed "s/^ *//" | cut -d" " -f1`
  fi
fi

# Data used for Reporting
PASS=$PWD
ok=0
fail=0
target_launched=0

echo "" > $PASS/ReportQA
echo "---------------------------- Report emul_qa.bash ----------------------------" >> $PASS/ReportQA

# Loop to launch stimuli
for((i=1;i<=$max_line;i=i+1))
do
  # Current working line, remove commentary lines
  if [ $stimulu_to_use == "none" ]
  then
    line=`grep -v "^ *#" ./test.config | grep "|" | head -n$i | tail -1 | sed "s/\*/star_remplaced/g"`
  else
    if [ $tg_comp_to_use != "none" ]
    then
      line=`grep -v "^ *#" ./test.config | grep "|" | grep "^ *$stimulu_to_use|\|^ *$stimulu_to_use  *|" | grep -w "$tg_comp_to_use" | head -n$i | tail -1 | sed "s/\*/star_remplaced/g"`
    else
      line=`grep -v "^ *#" ./test.config | grep "|" | grep "^ *$stimulu_to_use|\|^ *$stimulu_to_use  *|" | head -n$i | tail -1 | sed "s/\*/star_remplaced/g"`
    fi
  fi

  empty_line=`echo "${line}" | wc -c`
  if [ $empty_line -gt 1 ]
  then
    # Set all variables using current line
    stimulu=`echo $line | cut -d"|" -f1 | sed "s/ *//g"`
    if [ $tg_comp_to_use == "none" ]
    then
      tg_comp=`echo $line | cut -d"|" -f2 | sed "s/ *//g" | sed "s/\t//g" | sed "s/\*//"` 
    else
      tg_comp=$tg_comp_to_use
    fi
    if [ $tg_run_to_use == "none" ] || [ $# -ne 4 ]
    then
      tg_run=`echo $line | cut -d"|" -f3 | sed "s/ *//g" | sed "s/\t//g" | sed "s/\*//"` 
    else
      tg_run=$tg_run_to_use
    fi
    hw_description=`echo $line | cut -d"|" -f4 | sed "s/ *//g"`
    tool=`echo $line | cut -d"|" -f5 | sed "s/ *//g"`
    run_timeout=`echo $line | cut -d"|" -f6 | sed "s/ *//g"`
  
    if [ -n "$tg_comp" ]
    then
      if [ $tg_comp == "NTE" ]
      then
        if [ $makefile_target == "compil" ] || [ $makefile_target == "compil_gui" ] 
        then
          echo ""  
          echo "  TG_COMP is equal to NTE so no Compil. can be performed."  
          echo ""  
          exit 0
        fi
      fi
    fi

    if [ -n "$tg_run" ]
    then
      if [ $tg_run == "NTE" ]
      then
        if [ $makefile_target == "run" ]
        then
          echo ""  
          echo "  TG_RUN is equal to NTE so no RUN can be performed."  
          echo ""  
          exit 0
        fi
      fi
    fi
 
    # if a stimulu is defined , launch only it
    if [ $# -eq 2 ]
    then
      if [ $stimulu == $stimulu_to_use ]
      then
        if [ $tg_comp == "star_remplaced" ]
        then
          export TG_COMP=""
        else
          export TG_COMP=$tg_comp
        fi
        if [ $tg_run == "star_remplaced" ]
        then
          export TG_RUN=""
        else
          export TG_RUN=$tg_run
        fi
   
        t0=`date +%s`
        target_launched=`expr $target_launched + 1`
        make -C $stimulu $makefile_target
        
        if [ $? -ne 0 ]
        then
          t1=`date +%s`;
          echo "STIMULU : $stimulu | TG_COMP : $TG_COMP | TG_RUN : $TG_RUN | FAILED | time = `expr $t1 - $t0`" >> $PASS/ReportQA
          fail=`expr $fail + 1`
        else 
          t1=`date +%s`;
          echo "STIMULU : $stimulu | TG_COMP : $TG_COMP | TG_RUN : $TG_RUN | OK | time = `expr $t1 - $t0`" >> $PASS/ReportQA
          ok=`expr $ok + 1`
        fi

      fi
    else

      if [ $tg_comp == "star_remplaced" ]
      then
        export TG_COMP=""
      else
        export TG_COMP=$tg_comp
      fi
      if [ $tg_run == "star_remplaced" ]
      then
        export TG_RUN=""
      else
        export TG_RUN=$tg_run
      fi

      t0=`date +%s`
      target_launched=`expr $target_launched + 1`
      make -C $stimulu $makefile_target

      if [ $? -ne 0 ];
      then 
        t1=`date +%s`; 
        echo "STIMULU : $stimulu | TG_COMP : $TG_COMP | TG_RUN : $TG_RUN | FAILED | time = `expr $t1 - $t0`" >> $PASS/ReportQA
        fail=`expr $fail + 1`;
      else 
        t1=`date +%s`; 
        echo "STIMULU : $stimulu | TG_COMP : $TG_COMP | TG_RUN : $TG_RUN | OK | time = `expr $t1 - $t0`" >> $PASS/ReportQA
        ok=`expr $ok + 1`;
      fi

    fi
  fi
done

# Reporting
if [ -f $PASS/ReportQA ]
then 
  echo "-----------------------------------------------------------------------" >> $PASS/ReportQA
  echo " " >> $PASS/ReportQA
  echo " " >> $PASS/ReportQA
  echo "---------------------------- Final Report  ----------------------------" >> $PASS/ReportQA
  echo "RESULTS OK : $ok / $target_launched"   >> $PASS/ReportQA
  echo "RESULTS KO : $fail / $target_launched" >> $PASS/ReportQA
  echo "-----------------------------------------------------------------------" >> $PASS/ReportQA
  cat $PASS/ReportQA
  echo "---------------------------- TEST INFOS  ------------------------------" >> $PASS/ReportQA
  echo "Test Path : $PASS " >> $PASS/ReportQA
  cat $PASS/ReportQA | mail -s "[QA_DESK] $PASS results" $USER@eve-team.com
fi
