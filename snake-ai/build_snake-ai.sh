#  g++ -std=c++11 -o cool-la_gui cool-la_gui.cxx cool-la.cpp -I/home/smathews/fltk-1.3.4-2/ -L/home/smathews/fltk-1.3.4-2/ -lfltk -lX11 -ldl -lXext -lXft -lfontconfig -lXrender -lpthread -lXfixes


#g++ -std=c++11 -o cool-la_gui cool-la_gui.cxx cool-la.cpp -lfltk -lX11 -ldl -lXext -lXft -lfontconfig -lXrender -lpthread -lXfixes -lXcursor -lXinerama


#g++ -std=c++11 -o cool-la_gui cool-la_gui.cxx cool-la.cpp /home/smathews/dlib-19.15/dlib/all/source.cpp -ldlib -lfltk -lX11 -ldl -lXext -lXft -lfontconfig -lXrender -lpthread -lXfixes -lXcursor -lXinerama -lpng -ljpeg -lblas -llapack -I/home/smathews/dlib-19.15/ -L/home/smathews/dlib-19.15/examples/build/dlib_build/


g++ -std=c++11 -o snake-ai_gui snake-ai-gui.cxx snake-ai.cpp neuralnetwork.cpp /home/smathews/dlib-19.17/dlib/all/source.cpp -ldlib -lfltk -lX11 -ldl -lXext -lXrender -lpthread -lXfixes -lpng -ljpeg -lblas -llapack -I/home/smathews/dlib-19.17/ -L/home/smathews/dlib-19.17/examples/build/dlib_build/



