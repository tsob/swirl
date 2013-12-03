//----------------------------------------------------------------------------
// Name: swirl-networking.cpp
// Desc: Networking and OSC stuff
//
// Author: Tim O'Brien    (tsob@ccrma.stanford.edu)
//         Reza Payami  (rpayami@ccrma.stanford.edu)
//         Haley Sayres    (hsayres@stanford.edu)
// Date:   Fall 2013
//----------------------------------------------------------------------------

#include "swirl-networking.h"
#include <pthread.h> //TODO necessary?
#include "swirl-globals.h"
#include "UdpSocket.h" //TODO
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"

using namespace std;

struct NetworkLocation
{
    const char address[15];
    int port;
};

//-----------------------------------------------------------------------------
// Name: class ExamplePacketListener
// Desc: our packet listener
//-----------------------------------------------------------------------------
class SWIRLPacketListener : public osc::OscPacketListener {
protected:

    virtual void ProcessMessage( const osc::ReceivedMessage& m,
                                const IpEndpointName& remoteEndpoint )
    {
        (void) remoteEndpoint; // suppress unused parameter warning
        osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
        SWIRLAvatar* otherAvatar = (SWIRLAvatar*)getAvatar();
        float f;
        
        args >> f >> osc::EndMessage;
        std::cout << "Receiving " << m.AddressPattern() << " " << f << std::endl;

        try{
            if( std::strcmp( m.AddressPattern(), "/move" ) == 0 ){
                otherAvatar->move(f);
            }
            else if( std::strcmp( m.AddressPattern(), "/turn" ) == 0 ){
                otherAvatar->turn(f);
            }
            else if( std::strcmp( m.AddressPattern(), "/strafe" ) == 0 ){
                otherAvatar->strafe(f);
            }
        }catch( osc::Exception& e ){
            std::cout << "error while parsing message: "
            << m.AddressPattern() << ": " << e.what() << "\n";
        }
    }
};


//-----------------------------------------------------------------------------
// Name: oscListener()
// Desc: ...
//-----------------------------------------------------------------------------
void * oscListener(void * args)
{
    cerr << "Starting listening thread!" << endl;
    SWIRLPacketListener listener;

    NetworkLocation *  receiveLocation = (NetworkLocation*)args;

    UdpListeningReceiveSocket s(
        IpEndpointName(
          //IpEndpointName::ANY_ADDRESS,
          receiveLocation->address,
          receiveLocation->port
          ),
        &listener
        );

    //s.RunUntilSigInt();
    s.Run();

    pthread_exit(NULL);
    cerr << "Exiting thread!" << endl;
}

//TODO
//-----------------------------------------------------------------------------
// Name: transmitSocket
// Desc: ...
//-----------------------------------------------------------------------------
/*static*/ UdpTransmitSocket* transmitSocket = NULL;

//TODO
//-----------------------------------------------------------------------------
// Name: getTransmitSocket()
// Desc: ...
//-----------------------------------------------------------------------------
UdpTransmitSocket* getTransmitSocket()
{
    return transmitSocket;
}

//TODO
//-----------------------------------------------------------------------------
// Name: getTransmitSocket(const char* address, int port)
// Desc: ...
//-----------------------------------------------------------------------------
UdpTransmitSocket* getTransmitSocket(const char* address, int port)
{
    //TODO
    if (!transmitSocket)
        transmitSocket = new UdpTransmitSocket( IpEndpointName(address, port) );
    return transmitSocket;

}

//-----------------------------------------------------------------------------
// Name: swirl_send_message(char* label, float value)
// Desc: send an osc message
//-----------------------------------------------------------------------------
void * swirl_send_message(const char* label, float value)
{
   UdpTransmitSocket* transmitSocket = getTransmitSocket();

   char buffer[SWIRL_FRAMESIZE];
   osc::OutboundPacketStream oscOutstream( buffer, SWIRL_FRAMESIZE);

   oscOutstream
     << osc::BeginBundleImmediate
     << osc::BeginMessage( label )
     << value
     << osc::EndMessage
     << osc::EndBundle;

   transmitSocket->Send( oscOutstream.Data(), oscOutstream.Size() );
}

