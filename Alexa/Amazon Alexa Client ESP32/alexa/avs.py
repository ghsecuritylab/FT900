import socket
#import uselect
import struct
import os
import time
#import heapq
import uheapq as heapq



############################################################################################
# configuration
############################################################################################
CONFIG_SERVER_IP                     = '192.168.100.12'
#CONFIG_SERVER_IP                    = '192.168.0.125'
CONFIG_SERVER_PORT                   = 11234
CONFIG_BUFFER_SEND_SIZE              = 512
CONFIG_BUFFER_RECV_SIZE              = 512
CONFIG_RECV_TIMEOUT_MS               = 1000
CONFIG_RECV_TIMEOUT_MS_2             = 10000

############################################################################################
# Device capabilities
############################################################################################
DEVICE_CAPABILITIES_OFFSET_FORMAT    = 0
DEVICE_CAPABILITIES_OFFSET_BITDEPTH  = 4
DEVICE_CAPABILITIES_OFFSET_BITRATE   = 8
DEVICE_CAPABILITIES_OFFSET_CHANNEL   = 12
DEVICE_CAPABILITIES_MASK_FORMAT      = 0xF
DEVICE_CAPABILITIES_MASK_BITDEPTH    = 0xF
DEVICE_CAPABILITIES_MASK_BITRATE     = 0xF
DEVICE_CAPABILITIES_MASK_CHANNEL     = 0xF
DEVICE_CAPABILITIES_FORMAT_RAW       = 0
DEVICE_CAPABILITIES_FORMAT_MP3       = 1
DEVICE_CAPABILITIES_FORMAT_WAV       = 2
DEVICE_CAPABILITIES_FORMAT_AAC       = 3
DEVICE_CAPABILITIES_BITDEPTH_8       = 0
DEVICE_CAPABILITIES_BITDEPTH_16      = 1
DEVICE_CAPABILITIES_BITDEPTH_24      = 2
DEVICE_CAPABILITIES_BITDEPTH_32      = 3
DEVICE_CAPABILITIES_BITRATE_16000    = 0
DEVICE_CAPABILITIES_BITRATE_32000    = 1
DEVICE_CAPABILITIES_BITRATE_44100    = 2
DEVICE_CAPABILITIES_BITRATE_48000    = 3
DEVICE_CAPABILITIES_BITRATE_96000    = 4
DEVICE_CAPABILITIES_BITRATE_8000     = 5
DEVICE_CAPABILITIES_CHANNEL_1        = 0
DEVICE_CAPABILITIES_CHANNEL_2        = 1
CONF_AUDIO_SEND_FORMAT               = DEVICE_CAPABILITIES_FORMAT_RAW
CONF_AUDIO_SEND_BITDEPTH             = DEVICE_CAPABILITIES_BITDEPTH_16
CONF_AUDIO_SEND_BITRATE              = DEVICE_CAPABILITIES_BITRATE_16000
CONF_AUDIO_SEND_CHANNEL              = DEVICE_CAPABILITIES_CHANNEL_1
CONF_AUDIO_RECV_FORMAT               = DEVICE_CAPABILITIES_FORMAT_RAW
CONF_AUDIO_RECV_BITDEPTH             = DEVICE_CAPABILITIES_BITDEPTH_16
CONF_AUDIO_RECV_BITRATE              = DEVICE_CAPABILITIES_BITRATE_16000
CONF_AUDIO_RECV_CHANNEL              = DEVICE_CAPABILITIES_CHANNEL_1

############################################################################################
# avs class
############################################################################################
class avs:

    def __init__(self):
        self.handle = None
        self.quits = False
        self.poller = None
        self.queue_data = None
        self.buffer_send = None

    def avs_set_capabilities(self, format, depth, rate, channel):
        cap = 0
        cap |= (format  & DEVICE_CAPABILITIES_MASK_FORMAT)   << DEVICE_CAPABILITIES_OFFSET_FORMAT
        cap |= (depth   & DEVICE_CAPABILITIES_MASK_BITDEPTH) << DEVICE_CAPABILITIES_OFFSET_BITDEPTH
        cap |= (rate    & DEVICE_CAPABILITIES_MASK_BITRATE)  << DEVICE_CAPABILITIES_OFFSET_BITRATE
        cap |= (channel & DEVICE_CAPABILITIES_MASK_CHANNEL)  << DEVICE_CAPABILITIES_OFFSET_CHANNEL
        return (cap & 0xFFFF)

    def connect(self, device_id):

        self.handle = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sockaddr = socket.getaddrinfo(CONFIG_SERVER_IP, CONFIG_SERVER_PORT)[0][-1]

        # connect to server
        try:
            self.handle.connect(sockaddr)
        except:
            self.handle.close()
            self.handle = None
            return False

        # Set device info
        buf = bytes()
        buf += struct.pack('I', device_id)
        sendCap = self.avs_set_capabilities(CONF_AUDIO_SEND_FORMAT, CONF_AUDIO_SEND_BITDEPTH, CONF_AUDIO_SEND_BITRATE, CONF_AUDIO_SEND_CHANNEL)
        buf += struct.pack('H', sendCap)
        recvCap = self.avs_set_capabilities(CONF_AUDIO_RECV_FORMAT, CONF_AUDIO_RECV_BITDEPTH, CONF_AUDIO_RECV_BITRATE, CONF_AUDIO_RECV_CHANNEL)
        buf += struct.pack('H', recvCap)

        # Send device info
        self.handle.sendall(buf)

        # Initialize variables
        self.queue_data = []
        heapq.heapify(self.queue_data)
        self.buffer_send = bytearray(CONFIG_BUFFER_SEND_SIZE)
        return True

    def disconnect(self):
        if self.handle is None:
            return
        self.handle.close()
        self.handle = None
        return

    def is_connected(self):
        if self.handle is None:
            return False
        return True

    def send_audio(self, file_name):

        # get file size
        statinfo = os.stat(file_name)
        if not statinfo:
            return
        file_size = statinfo[6]
        if file_size == 0:
            return

        # open file
        file = open(file_name, "r")
        if not file:
            print("file does NOT exists {}".format(file_name))
            return

        # send size of file to send
        val = struct.pack('I', file_size)
        self.handle.send(val)
        #print(file_size)

        # send file in chunks
        total_size = 0
        while True:
            # read bytes into buffer
            read_size = file.readinto(self.buffer_send)
            if read_size == 0:
                break

            # send buffer
            if read_size == CONFIG_BUFFER_SEND_SIZE:
                self.handle.send(self.buffer_send)
            else:
                # note: sending part of buffer allocates memory
                self.handle.send(self.buffer_send[:read_size])
            total_size += read_size
            # print(total_size)

            #send_size = int(read_size/2)
            #short16s = struct.unpack('h'*send_size, buffer_send[:])
            #for i in range(send_size):
            #    buffer_send[i] = u_law_e(short16s[i])
            #self.handle.sendall(buffer_send[:send_size])
            #print(send_size)

        # close file
        file.close()
        return True

    def recv_and_play_audio(self):

        # set receive timeout
        self.handle.settimeout(CONFIG_RECV_TIMEOUT_MS)
        #if self.poller is None:
        #    self.poller = uselect.poll()
        #    self.poller.register(self.handle, uselect.POLLIN)
        #res = self.poller.poll(CONFIG_RECV_TIMEOUT_MS)
        #if not res:
        #    return True

        # receive the size of audio segment
        try:
            val = self.handle.recv(4)
            if not val:
                #print("Recv size ERROR")
                return False
            total_size = int(struct.unpack('I', val[:])[0])
            print(total_size)
        except OSError:
            return True
        except:
            return False

        want_recv = CONFIG_BUFFER_RECV_SIZE
        total_recv = 0

        # receive the audio segment
        while total_recv < total_size:
            if total_size-total_recv < want_recv:
                want_recv = total_size-total_recv

            # receive audio segment
            self.handle.settimeout(CONFIG_RECV_TIMEOUT_MS_2)
            try:
                val = self.handle.recv(want_recv)
                if not val:
                    print("Recv size ERROR2")
                    return False
                total_recv += len(val)
                print(total_recv)
            except OSError:
                return True
            except:
                return False

        return True

    def recv_audio(self):
        # set receive timeout
        self.handle.settimeout(CONFIG_RECV_TIMEOUT_MS)

        # receive the size of audio segment
        try:
            val = self.handle.recv(4)
            if not val:
                print("Recv size ERROR")
                return False
            total_size = int(struct.unpack('I', val[:])[0])
        except OSError:
            return True
        except:
            print("Recv size ERROR2")
            return False

        want_recv = CONFIG_BUFFER_RECV_SIZE
        total_recv = 0
        print("streaming {} bytes".format(total_size))

        # receive the audio segment
        while total_recv < total_size:
            if total_size-total_recv < want_recv:
                want_recv = total_size-total_recv

            # receive audio segment
            self.handle.settimeout(CONFIG_RECV_TIMEOUT_MS_2)
            try:
                data = self.handle.recv(want_recv)
                if not data:
                    print("Recv size ERROR3")
                    return False
                total_recv += len(data)
                #print(total_recv)
            except OSError:
                return True
            except MemoryError:
                print("Recv size ERRORX")
                return True
            except:
                print("Recv size ERROR4")
                return False

            # queue the data
            try:
                heapq.heappush(self.queue_data, data)
                print("push {}".format(len(data)))
                #time.sleep(0.010)
            except:
                print("push {} failed!".format(len(data)))
                return False

        print("streamed  {} bytes".format(total_recv))
        return True

    def play_audio(self):
        if self.queue_data is None:
            return False
        try:
            # dequeue a data
            data = heapq.heappop(self.queue_data)
            print("pop {}".format(len(data)))

            # TODO: play data
        except:
            #print("play_audio exception ")
            return False
        return True

    def is_quit(self):
        return self.quits

    def quit(self):
        self.quits = True

    def get_server_ip(self):
        return CONFIG_SERVER_IP

    def get_server_port(self):
        return CONFIG_SERVER_PORT

