(function(){

    function Grid(xSize, ySize){
        var me = this;

        me.xSize = xSize || 10;
        me.ySize = ySize || 10;
        me.grid = [];
        me.blockList = {};

        me.init();
    }

    var prototype = {

        init: function(){
            var me = this
                , grid = me.grid
                ; 

            for(var i=0; i<me.ySize; i++){
                grid.push([]);
                for(var j=0; j<me.xSize; j++){
                    grid[i].push(0);
                }
            }
        }

        , getBlockRect: function(id){
            return this.blockList[id];
        }

        , getMinRect: function(){
            var me = this
                , rect = null
                , grid = me.grid
                , i, j
                , wMax = 0
                , hMax = 0
                ;

            for(i=0; i<me.ySize; i++){
                for(j=0; j<me.xSize; j++){
                    if(grid[i][j]){
                        wMax = Math.max(j + 1, wMax);
                        hMax = Math.max(i + 1, hMax);
                    }
                }
            } 

            if(wMax > 0 && hMax > 0){
                rect = {
                    x: 0
                    , y: 0
                    , w: wMax
                    , h: hMax
                }
            }

            return rect;
        }

        , getMaxSpareRect: function(minRect){
            var me = this
                , grid = me.grid
                , minRect = minRect || me.getMinRect()
                , rect
                , iStart, jStart
                , i, j
                , expandDirection
                , xRatio, yRatio
                ; 

            if(!minRect){
                rect = {
                    x: 0 
                    , y: 0
                    , w: me.xSize
                    , h: me.ySize
                };
            } 
            else {
                jStart = minRect.x + minRect.w - 1;
                iStart = minRect.y + minRect.h - 1;

                xRatio = minRect.w / me.xSize;
                yRatio = minRect.h / me.ySize;

                if(xRatio < yRatio){
                    expandDirection = 'X';
                }
                else if(yRatio < xRatio){
                    expandDirection = 'Y';
                }
                else {
                    expandDirection = 'XY';
                }

                if(grid[iStart][jStart]){
                    if(iStart < me.ySize - 1 && expandDirection != 'X'){
                        iStart++;
                    }

                    if(jStart < me.xSize - 1 && expandDirection != 'Y'){
                        jStart++;
                    }

                    if(iStart == me.ySize - 1 && jStart == me.xSize - 1) {
                        rect = null;
                    }
                }

                var w
                    , h
                    , wMax = Infinity
                    , areas = [];

                h = 0;

                for(i=iStart; i >= 0; i--){
                    h++;
                    w = 0;
                    for(j=jStart; j>=0; j--){
                        w++; 
                        if(grid[i][j]){
                            wMax = Math.min(w - 1, wMax);
                            areas.push({
                                x: jStart - wMax + 1 
                                , y: i 
                                , w: wMax
                                , h: h
                                , area: wMax * h  
                            });
                            break;
                        }
                    }
                    if(j < 0){
                        wMax = Math.min(w, wMax);
                        areas.push({
                            x: 0
                            , y: i 
                            , w: wMax
                            , h: h
                            , area: wMax * h  
                        });
                    }
                }

                if(rect !== null){
                    areas.sort(function(a, b){
                        return b.area - a.area;
                    });
                    rect = areas[0];
                }
            }

            return rect;
        }

        , placeBlock: function(id, block, debug){
            var me = this
                , minRect = me.getMinRect()
                , maxSpareRect
                , pos = {x: 0, y: 0}
                ;

            if(!minRect){
                pos.x = 0;
                pos.y = 0;
            }
            else {
                maxSpareRect = me.getMaxSpareRect(minRect);
                if(!maxSpareRect) {
                    pos.x = minRect.x + minRect.w;
                    pos.y = 0;
                }
                else {
                    pos.x = maxSpareRect.x;
                    pos.y = maxSpareRect.y;
                }
            }

            me.addBlock(id, pos, block, debug); 
        } 

        , addBlock: function(id, pos, block, debug){
            var me = this
                , grid = me.grid
                ;

            for(var i=pos.y; i<pos.y + block.h && i<me.ySize; i++){
                for(var j=pos.x; j<pos.x + block.w && j<me.xSize; j++){
                    if(!grid[i][j]){
                        grid[i][j] = debug ? id : 1;
                    }
                    else {
                        throw new Error('grid[' + i + '][' + j + '] is occupied!');
                    }
                }
            }     
            block.gridPos = pos;
            me.blockList[id] = block; 
        }

    };

    function extend(dest, src){
        for(var i in src){
            dest[i] = src[i];
        }
    }

    extend(Grid.prototype, prototype);

    window.Grid = Grid;

})(); 
