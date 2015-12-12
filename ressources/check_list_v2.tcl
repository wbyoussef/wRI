#!/usr/bin/tclsh

# define a global variable
set listfinal ""
set handler [open "ficresultat.log" w ]
# procedure to traverse a tree       
proc Parcours_Arborescence { repertoire } {
 # declare variables    
 global ListeRep ListeFic listfinal TestName

     cd $repertoire
     set path [pwd]
     set repertoire $path
     set ListeRep ""
     set ListeFic ""
     set contenu [glob -nocomplain *]

     # Discrimination directory / file
     foreach element $contenu {
         if [file isdirectory $element] {
             lappend ListeRep $element
         } else {
             lappend ListeFic $element
         }
     }

     # store the files in the global variable
     foreach fic $ListeFic { 
             lappend  listfinal "$repertoire/$fic"
            
     }

     # For directories, they muset be traveled
     # Test for existence of a directory otherwise quit the current directory 
     if { [llength $ListeRep]  == 0 } {
             cd ".."
             # update the path
             set path [pwd]
        } else {
                 foreach rep $ListeRep {
                         # call the procedure
                         Parcours_Arborescence  [file join $path $rep]
                         }
               }              
}  

# procedure for searching a word in a file 
proc grep {fich,word,num_request} {
 # declare variables
 global fich word num_request handler
 
      # open file and travel it line by line 
      set fic [open $fich r]
      while {[gets $fic line] >= 0} {
         if { [regexp $word  $line]} {
               puts $handler "Request $num_request is valid"
               break
               }
            }
          if { [eof $fic] ==1 } {
               puts  $handler  "Request  $num_request is not  valid"
            }
}

# define the procedures of check

proc CK_MKF_001 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*Makefile" ] ]
  set word "REMOTE"
  set num_request "CK_MKF_001"
  grep $fich,$word,$num_request
}

proc CK_MKF_006 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*Makefile" ] ]
  set word "compil_gui"
  set num_request "CK_MKF_006"
  grep $fich,$word,$num_request
}

proc CK_MKF_009 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*Makefile" ] ]
  set word "run"
  set num_request "CK_MKF_009"
  grep $fich,$word,$num_request
}

proc CK_MKF_004 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*Makefile" ] ]
  set word "COMP Stimuli has no RUN Target."
  set num_request "CK_MKF_004"
  grep $fich,$word,$num_request
}


proc CK_ZPF_001 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*zpf" ] ]
  set word "FILE_CONF"
  set num_request "CK_ZPF_001"
  grep $fich,$word,$num_request

}

proc CK_ZPF_003 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*zpf" ] ]
  set word "ZCUIWORK"
  set num_request "CK_ZPF_003"
  grep $fich,$word,$num_request
}

proc REMOTECMD_is_set { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*zpf" ] ]
  set word "REMOTECMD"
  set num_request " {REMOTECMD is set} "
  grep $fich,$word,$num_request
}

proc REMOTESYN_is_set { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*zpf" ] ]
  set word "REMOTESYN"
  set num_request "{REMOTESYN is set} "
  grep $fich,$word,$num_request
}

proc CK_ZPF_004 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*zpf" ] ]
  set word "MaximumNumberOfExistPRetry"
  set num_request "CK_ZPF_004 "
  grep $fich,$word,$num_request
}



proc CK_RM_005 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*README" ] ]
  set word "Author : * "
  set num_request "CK_RM_005"
  grep $fich,$word,$num_request
}

proc CK_RM_006 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*README" ] ]
  set word "Contact: * "
  set num_request "CK_RM_006"
  grep $fich,$word,$num_request
}

proc CK_RM_007 { } {
  global listfinal fich word num_request
  set fich [lindex $listfinal [lsearch  $listfinal  "*README" ] ]
  set word "Date   : */*/* "
  set num_request "CK_RM_007"
  grep $fich,$word,$num_request
}


####################main####################

# test the number of inputs
set TestName [lrange $argv 0 end ]
if { [llength $TestName] != 1} {
    puts " Please insert the test name, it must be only one argument ! "
    exit -1
} 

# call "Parcours_Arborescence" procedure                    
Parcours_Arborescence [lindex $TestName  0]
          # Display list of files with absolute path"
          #foreach fic $listfinal {
          #   puts $fic
          #}

#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@MAKEFILE@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
puts $handler   "*************************MAKFILE REQUESTS*****************************"
CK_MKF_001  
CK_MKF_006
CK_MKF_009 
CK_MKF_004
#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ZPF@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
puts $handler   "*************************ZPF REQUESTS*********************************"
CK_ZPF_001
CK_ZPF_003
CK_ZPF_004
REMOTECMD_is_set
REMOTESYN_is_set
#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@README@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
puts $handler   "*************************README REQUESTS******************************"
CK_RM_005
CK_RM_006
CK_RM_007 

# close result file 
close $handler



    




