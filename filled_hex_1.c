    if (data.filled == 1) {
        coords k1, k2;
        k1=l1;
        k2=l2;
        k1.y=l1.y-1;
        k2.y=l2.y-1;
        drawingLine(k1, k2, data.fill_color, data.thickness, img);
            while(k1.y!= l6.y && k2.y != l3.y){
                k1.y-=1;
                k2.y-=1;
                drawingLine(k1, k2, data.fill_color, data.thickness, img);
                
            }
            
    }
