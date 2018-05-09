#include "sample_common.h"


class Board;
Board *g_board; // ゲーム盤
StatusLine *g_statusline; // NEXT:表示をするためのスプライト

int g_turn; // 2で割って余りが0なら黒、1なら白
#define IS_BLACK_TURN(n) ( (n%2) == 0 )

/////////////////////
class Board : public Prop2D {
public:
    Grid *bg; // 背景の緑色の板面
    Grid *fg; // 黒や白のピースを保持する
    static const int BLACK=0, WHITE = 1, NONE=-1, OUTSIDE=-2;
    Board() : Prop2D() {
        setScl(48); 
        bg = new Grid(8,8);
        addGrid(bg);
        fg = new Grid(8,8);
        addGrid(fg);

        setDeck(g_base_deck);
        setLoc( -8*48/2,-8*48/2);

        // initial
        setPiece(3,3,BLACK);
        setPiece(4,4,BLACK);
        setPiece(3,4,WHITE);
        setPiece(4,3,WHITE);        

        // bg
        for(int i=0;i<8;i++) {
            for(int j=0;j<8;j++) {
                bg->set(i,j,ATLAS_GREEN_BG);
            }
        }
        g_main_layer->insertProp(this);
    }
    void setPiece(int x,int y,int col) {
        if(x<0||y<0||x>=8||y>=8)return;
        if(col==NONE) fg->set(x,y,Grid::GRID_NOT_USED); else fg->set(x,y,col==WHITE?ATLAS_PIECE_WHITE:ATLAS_PIECE_BLACK);        
    }
    int getPiece(int x, int y) {
        if(x<0||y<0||x>=8||y>=8) return OUTSIDE;
        int ind = fg->get(x,y);
        if(ind==Grid::GRID_NOT_USED) 
            return NONE;
        else
            return ind == ATLAS_PIECE_WHITE?WHITE : BLACK;
    }
};

void updateStatusLine() {
    CharGrid *cg = g_statusline->getCharGrid();
    cg->printf(0,0,Color(1,1,1,1), IS_BLACK_TURN(g_turn) ? "NEXT: BLACK" : "NEXT: WHITE" );    
}


void reverseInit() {
    g_board = new Board();

    g_statusline = new StatusLine();
    g_statusline->setLoc(SCRW/2-300,SCRH/2-24);
    g_effect_layer->insertProp(g_statusline);

    updateStatusLine();
}
void reverseUpdate() {
    // Convert mouse position to board cell coord
    Vec2 mpos = g_mouse->getCursorPos();
    mpos -= Vec2(SCRW/2,-SCRH/2);
    mpos.y = SCRH-mpos.y;
    Vec2 relloc = mpos - g_board->loc;
    int bx = relloc.x / 48;
    int by = relloc.y / 48;

    // Left click to put/turn
    if( g_mouse->getToggled(0) ) {
        g_mouse->clearToggled(0);
        print("mouse 0 press:%f,%f bpos:%d,%d",mpos.x,mpos.y, bx,by);
        int curcol = g_board->getPiece(bx,by);
        if(curcol==Board::OUTSIDE) {
            return;
        } else if( curcol != Board::NONE) {
            print("turnover:%d",curcol);
            g_board->setPiece(bx,by, curcol == Board::WHITE ? Board::BLACK : Board::WHITE );
        } else {
            print("put new");            
            int putcol;
            if(IS_BLACK_TURN(g_turn)) {
                putcol = Board::BLACK;
            } else {
                putcol = Board::WHITE;
            }
            g_board->setPiece(bx,by,putcol);
            g_turn++;
            updateStatusLine();                    
        }
    }
    // Right click to clear
    if( g_mouse->getToggled(1) ) {
        print("rmousedow");
        g_mouse->clearToggled(1);
        g_board->setPiece(bx,by,Board::NONE);
    }    
}


SAMPLE_COMMON_MAIN_FUNCTION(reverseInit,reverseUpdate,"reversi");
