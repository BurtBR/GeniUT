clear;clc;close;

charvector = ["C","Cs","D","Ds","E","F","Fs","G","Gs","A","As","B"];

freq = 440;
Fs = 44100;
dt = 1/Fs;
cuttreshold = 0.7;
scale = 2;

time = 0.5;
t = (0:dt:time);

fadeout = ones(1,length(t));
fadein = ones(1,length(t));

1/t(2000)
fadein(1:2000) = (t(1:2000)*22.0610);
fadeout(18000:22051) = exp(-20*(0:0.0001:0.4051));

notecounter = -33;

for octave = 2:6
    for n = 1:12
        note = (freq*(2^((notecounter)/12)));
        notecounter = (notecounter + 1);
        filename = ("Sounds/"+int2str(octave)+charvector(n)+".wav");
        disp(filename);
        disp(note);
        sound = (fadein.*sin(2*pi*note*t-pi/2).*fadeout).*scale;
        sound(sound>cuttreshold) = cuttreshold;
        sound(sound<-cuttreshold) = -cuttreshold;
        plot(t, sound, 'LineWidth',3);
        ylim([-1.0 1.0]);
        player = audioplayer(sound/2, Fs);
        playblocking(player);
        audiowrite(filename,sound,Fs);
    end
end