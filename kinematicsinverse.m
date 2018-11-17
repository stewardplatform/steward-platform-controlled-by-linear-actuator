lmax=0;
lmin=100;

l1=4.0;     %平台参数
l2=2.0;

nod_a=20;    %运动范围设定
nod_down=-30;
nod_up=0;
shake=40;

fprintf("%f°摇头\n0°-%f° %f°-%f°点头\n",shake,nod_a,nod_down,nod_up);
a1=[-l1;-2*l1/sqrt(3);0];   %a为定平台连杆节点在oxyz坐标，b为动平台连杆节点在o1x1y1z1坐标
b1=[-l2/2;-4*l2/sqrt(3)+l2/2*sqrt(3);0];
a2=[l1;-2*l1/sqrt(3);0];
b2=[l2/2;-4*l2/sqrt(3)+l2/2*sqrt(3);0];
a3=[1.5*l1;-2*l1/sqrt(3)+l1/2*sqrt(3);0];
b3=[1.5*l2;2*l2/sqrt(3)-l2/2*sqrt(3);0];
a4=[l1/2;4*l1/sqrt(3)-l1/2*sqrt(3);0];
b4=[l2;2*l2/sqrt(3);0];
a5=[-l1/2;4*l1/sqrt(3)-l1/2*sqrt(3);0];
b5=[-l2;2*l2/sqrt(3);0];
a6=[-1.5*l1;-2*l1/sqrt(3)+l1/2*sqrt(3);0];
b6=[-1.5*l2;2*l2/sqrt(3)-l2/2*sqrt(3);0];
h=9.1;                  %脖子正常状态高度设定（佳佳脖子大概8cm长）
for A=[a1,a2,a3,a4,a5,a6] 
    if all(A==a1)
        bb=b1;
    elseif all(A==a2)
        bb=b2;
    elseif all(A==a3)
        bb=b3;
    elseif all(A==a4)
        bb=b4;
    elseif all(A==a5)
        bb=b5;
    else
        bb=b6;
    end
    
    for a=0*pi/180:pi/180:nod_a*pi/180           %0-30°的45°点头
        for b=nod_down*pi/180:pi/180:nod_up*pi/180                 %  45°点头 20
            c=0*pi/180;
            R=[ cos(a)*cos(c) - cos(b)*sin(a)*sin(c), - cos(a)*sin(c) - cos(b)*cos(c)*sin(a),  sin(a)*sin(b);
             cos(c)*sin(a) + cos(a)*cos(b)*sin(c),   cos(a)*cos(b)*cos(c) - sin(a)*sin(c), -cos(a)*sin(b);
              sin(b)*sin(c),cos(c)*sin(b),cos(b)];
            O1=[0;0;h];
            l=norm(R*bb+O1-A);
            if l>lmax
                lmax=l;
            end
            if l<lmin
                lmin=l;
            end
        end
    end
end

for A=[a1,a2,a3,a4,a5,a6]
    if all(A==a1)
        bb=b1;
    elseif all(A==a2)
        bb=b2;
    elseif all(A==a3)
        bb=b3;
    elseif all(A==a4)
        bb=b4;
    elseif all(A==a5)
        bb=b5;
    else
        bb=b6;
    end
    for a=-shake*pi/180.0:pi/180:shake*pi/180.0                 % 40°摇头
        b=0*pi/180.0;
        c=0*pi/180;
        R=[ cos(a)*cos(c) - cos(b)*sin(a)*sin(c), - cos(a)*sin(c) - cos(b)*cos(c)*sin(a),  sin(a)*sin(b);
             cos(c)*sin(a) + cos(a)*cos(b)*sin(c),   cos(a)*cos(b)*cos(c) - sin(a)*sin(c), -cos(a)*sin(b);
            sin(b)*sin(c),cos(c)*sin(b),cos(b)];
        O1=[0;0;h];
        l=norm(R*bb+O1-A);
        if l>lmax
           lmax=l;
        end
        if l<lmin
           lmin=l;
        end
    end
end
fprintf("lmin=%f\n",lmin*10);
fprintf("lmax=%f\n",lmax*10);
    


