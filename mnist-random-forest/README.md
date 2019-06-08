# dlib-examples/mnist-svm
Just some example dlib use of a multi-class SVM.  This example learns for the MNIST 60000 training set and tests against the 10000 testing set.  There is a prelearned model that is in the svm_mnist.dat file which can be passed as an argument to bypass the training.

##
Building and running
```
make -f Makefile.svm-mnist
time ./my_dlib_mnist-svm ./mnist svm_mnist.dat
```

Will render something like this:
```
                            
       ##     ##            
       ##     ##            
       ##     ##            
       ##      ##           
       ##      ##           
       ##      ##           
       ###     ##           
        ##########          
        ##########          
          * *## ##          
                 #          
                 #          
                 ##         
                 ##         
                 ##         
                 ##         
                 *##        
                 -###       
                  ###       
                  ###       
                            
                            
predicted label: 9, true label: 4
                            
                            
                            
                            
                            
         ######             
       #########            
      ###########           
      #####   *##*          
     ####      ###          
     ####      ###          
     ####     ####          
       ###-*######          
       #############        
        ##############      
         #####    ####      
         ###       ####     
        ###        ####     
        ###        ####     
         ###      #####     
         ###      #####     
          ####  ######      
           ##########*      
            ########        
             ######*        
                            
                            
                            
predicted label: 3, true label: 8
                            
                            
                            
                            
                            
             ###            
            #####           
            ## ###          
            #   ###         
                ##          
                ##          
               ###          
             *###           
            ####            
           ###              
          ###-              
         ####               
         #####              
         #######            
            #####           
               ###          
               ###          
               ###          
               ###          
              ####          
                            
                            
                            
predicted label: 1, true label: 3
                            
                            
                            
                            
         #####              
         #######            
             ####           
                ##          
                 #          
                 #          
                 #          
                ##          
                #           
               ##           
              ##            
             *#*            
             ##             
            ##              
           ##               
           ##               
          ##                
          ##      ###       
          ####  ####        
           *######          
                            
                            
                            
                            
predicted label: 1, true label: 2
                            
                            
                            
                            
                            
                #####       
               #######      
              ###  ###      
              ##-  ###      
             ###  ###       
             ### ####       
             #######        
             ######         
             #####          
             ###*           
            ####            
           #####            
         -######            
         ###  ##            
        ####  ##            
        ###   ##            
        ###   ##            
        ###  ###            
         #######            
          #####             
                            
                            
                            
predicted label: 1, true label: 8
                            
                            
                            
                            
                            
              ###           
          ########          
      ######   ###          
    #####*     ###          
    ###       ###-          
              ###           
              ####          
              ###*          
              ###           
             ####           
             ####           
             ####    -      
         ###############    
     ###################    
    #####*   ###    ####    
     ##      ###            
             ###            
             ###            
             ##             
             -              
                            
                            
                            
predicted label: 2, true label: 7
                            
                            
                            
                            
                            
                   ##       
                ####-       
              ######        
             ######         
            ###             
            ##              
            ##  #           
           #######*         
           ########         
           ###   ##         
           ###   ##         
           ###   ##         
           ##   ###         
           ##   ##*         
           ##  ###          
              ###           
            ####            
          #####             
         #####              
         ###                
                            
                            
                            
predicted label: 9, true label: 5
                            
                            
                            
                            
                            
                            
                ##          
              #####         
             #######        
           ####*####        
           ### #####        
          ##########        
          ##########        
          -#########        
               ###*         
               ###          
              ####          
             ####           
             ###            
             ###            
            ###             
           ####             
          ####              
          ####              
         #####              
         ####               
                            
                            
predicted label: 7, true label: 9
                            
                            
                            
                            
                            
     #######                
     ##########             
      ######                
      ###                   
      ##*    ####           
      ##*  ########         
     ###############        
     ########     ###       
    #####         -###      
     ###           ###      
                   ####     
                    ###     
                    ###     
                    ###-    
                    ###-    
                   ####     
       ##         #####     
       ####     ######      
        #############-      
            ########        
                            
                            
                            
predicted label: 7, true label: 5
                            
                            
                            
                            
               *######      
              ########      
            ####   ###      
           ###     ###      
          ###      ###      
          ##       ##       
                  ##*       
                 ###        
                ##*         
               ###          
              ###           
             ###            
            ###             
          ###*              
         ###                
        ###         ##      
      -###       #####      
      ###    ########       
      ###########           
       ######               
                            
                            
                            
                            
predicted label: 8, true label: 2
testing num_right: 8933
testing num_wrong: 1067
testing accuracy:  0.8933

real	0m7.245s
user	0m7.222s
sys	0m0.624s

```
