
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(audio_engine_source_files 	AllpassDiffuser.h
	         					DelayLine.h
								FastSin.cpp
								ModulatedAllpass.h
								ModulatedDelay.h
								MultitapDiffuser.h
								Parameter.h
								ReverbChannel.h
								ReverbController.h
								Utils.h
								Utils/Sha256.cpp
								AudioLib/Biquad.cpp  
								AudioLib/Hp1.h  
								AudioLib/Lp1.h  
								AudioLib/ShaRandom.cpp  
								AudioLib/ValueTables.cpp)

add_library(audio_engine ${audio_engine_source_files})
