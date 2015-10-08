function [F,A,Phi] = load_sinusoid_tracks(path)
[fid,msg]=fopen(path,"r");
if (file == -1)
    error(['Error opening file: ' path msg]);
end
while (1)
    [params,count]=fread(file,4,'double');
    if (count != 4)
        break;
    end
    [track_number,count]=fread(file,1,'int');
    if (count != 1)
        break;
    end
    if (size(F,2) >= track_number)
        if (size(F{track_number},2) > 0)
            F{track_number} = [F{track_number} params(2)];
        end
            F{track_number} = [params(2)];
        end


