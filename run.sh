
#!/bin/bash
# Set audio device variable !!
#export QEMU_AUDIO_DRV=pa
#sudo qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 
#sudo qemu-system-x86_64 -device usb-audio -audiodev pa,id=hda,out.mixing-engine=off -audiodev pa,id=usb,server=unix:/tmp/pulse-socket

#!/bin/bash
# Set audio device variable !!

sudo qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -device ac97 -audiodev coreaudio,id=snd0


