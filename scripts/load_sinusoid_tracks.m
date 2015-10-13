function [T,F,A,Phi] = load_sinusoid_tracks(path)
[file,msg]=fopen(path,"r");
if (file == -1)
    error(['Error opening file: ' path msg]);
end
T={};
F={};
A={};
Phi={};
while (1)
    [params,count]=fread(file,4,'double');
    if (count != 4)
        break;
    end
    [track_number,count]=fread(file,1,'int');
    if (count != 1)
        break;
    end
    if (size(T,2) >= track_number)
        if (size(T{track_number},2) > 0)
            T{track_number} = [T{track_number} params(1)];
        else
            T{track_number} = [params(1)];
        end
    else
        T{track_number} = [params(1)];
    end
    if (size(A,2) >= track_number)
        if (size(A{track_number},2) > 0)
            A{track_number} = [A{track_number} params(2)];
        else
            A{track_number} = [params(2)];
        end
    else
        A{track_number} = [params(2)];
    end
    if (size(F,2) >= track_number)
        if (size(F{track_number},2) > 0)
            F{track_number} = [F{track_number} params(3)];
        else
            F{track_number} = [params(3)];
        end
    else
        F{track_number} = [params(3)];
    end
    if (size(Phi,2) >= track_number)
        if (size(Phi{track_number},2) > 0)
            Phi{track_number} = [Phi{track_number} params(4)];
        else
            Phi{track_number} = [params(4)];
        end
    else
        Phi{track_number} = [params(4)];
    end
end
fclose(file);
