if (data.filled == 1) {
        coords k1, k2;
        k1=l1;
        k2=l2;
        k1.y=l1.y-data.thickness;
        k2.y=l2.y-data.thickness;
        drawingLine(k1, k2, data.fill_color, data.thickness, img);
            while(k1.y!= l6.y && k2.y != l3.y){
                k1.y-=data.thickness;
                k2.y-=data.thickness;
                drawingLine(k1, k2, data.fill_color, data.thickness, img);  
            } 
            while(k1.y != l5.y+data.thickness && k2.y != l4.y+data.thickness ){
                k1.y-=data.thickness;
                k2.y-=data.thickness;
                drawingLine(k1, k2, data.fill_color, data.thickness, img); 
            }
            
    }
