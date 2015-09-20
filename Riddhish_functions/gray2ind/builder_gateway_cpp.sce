// This file is released under the 3-clause BSD license. See COPYING-BSD.

names = [	
		"gray2ind","opencv_gray2ind";
     		
		];

files = [
			"opencv_gray2ind.cpp",
     		
		];

function builder_gw_cpp()
    WITHOUT_AUTO_PUTLHSVAR = %t;
    copyfile("Riddhish_functions/gray2ind/common.h",TMPDIR);
    copyfile("Riddhish_functions/gray2ind/common.cpp",TMPDIR);
tbx_build_gateway("skeleton_imageprocessing", ..
	names, ..
    files, ..
    get_absolute_file_path("builder_gateway_cpp.sce"),[],["-lopencv_core -lopencv_highgui -lopencv_imgproc"],["-I/usr/local/include/opencv/ -I/usr/local/include/opencv2/"],"g++ -ggdb `pkg-config --cflags opencv` -o `basename opencv_imread.cpp .cpp` opencv_imread.cpp `pkg-config --libs opencv`");
endfunction

builder_gw_cpp();
clear builder_gw_cpp; // remove builder_gw_cpp on stack
