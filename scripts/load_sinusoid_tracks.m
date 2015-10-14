function [T,F,A,Phi] = load_sinusoid_tracks(path)
[file,msg]=fopen(path,"r");
if (file == -1)
    error(['Error opening file: ' path msg]);
end
floats=fread(file,Inf,'double');
track_nums=typecast(floats(5:5:end));

