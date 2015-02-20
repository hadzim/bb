#include "BB/Camera/CameraService.h"
#include "BB/App.h"
#include "BB/Camera/CameraSnapshooter.h"
#include "BB/Forwarder/BgForwarder.h"

//BB_MAIN(BB::CameraService)


namespace BB {


	class Factory : public IForwarderFactory {
		virtual IForwarder::Ptr createForwarder(){
			//create remote client

			IForwarder::Ptr camera1 = new CameraWorker("kitchen",
					new BB::FFMpegSnapshot("rtsp://192.168.0.124:554/mpeg4")
					//new BB::StaticSnapshot("http://192.168.99.85:9989/", "admin", "admin")
			);

			//forward on background
			BgForwarder::Ptr bg = new BgForwarder("kitchen", camera1);
			return bg;
		}
	};

	class Factory2 : public IForwarderFactory {
		virtual IForwarder::Ptr createForwarder(){
			//create remote client

			IForwarder::Ptr camera2 = new CameraWorker("room",
					new BB::FFMpegSnapshot("rtsp://192.168.0.123:554/mpeg4")
					//new BB::StaticSnapshot("http://192.168.99.85:9989/", "admin", "admin")
			);
			//forward on background
			BgForwarder::Ptr bg = new BgForwarder("room", camera2);
			return bg;
		}
	};
}

FWD_BB_MAIN2("Camera", BB::Factory, BB::Factory2)

/*
int main(int argc, char **argv) {
	std::cout <<" cam" << std::endl;
	TBS::initLogs("cam", 8);
	std::string path = "image.jpg";
	BB::FFMpegSnapshot s("rtsp://192.168.0.123:554/mpeg4");
	//BB::CameraSnapshooter snap("rtsp://admin:admin@192.168.99.85/cam/realmonitor?channel=1&subtype=0", path);
	//BB::("http://192.168.99.85:9989/", "admin", "admin")
	s.makeSnapshot("image.jpg", 640);
	std::cout <<" cam done" << std::endl;
	return 0;
}
*/



