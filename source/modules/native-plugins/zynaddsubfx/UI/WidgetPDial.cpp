// generated by Fast Light User Interface Designer (fluid) version 1.0107f

#include "WidgetPDial.h"
#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_Window.H>
#include "../Misc/Util.h"
//Copyright (c) 2003-2005 Nasca Octavian Paul
//License: GNU GPL version 2 or later

using namespace std;

class TipWin:public Fl_Menu_Window
{
    public:
        TipWin();
        void draw();
        void showValue(float f);
        void setText(const char *c);
        void showText();
    private:
        void redraw();
        const char *getStr() const;
        string tip;
        string text;
        bool   textmode;
};

TipWin::TipWin():Fl_Menu_Window(1, 1)
{
    set_override();
    end();
}

void TipWin::draw()
{
    //setup window
    draw_box(FL_BORDER_BOX, 0, 0, w(), h(), Fl_Color(175));
    fl_color(Fl_Tooltip::textcolor());
    fl_font(labelfont(), labelsize());

    //Draw the current string
    fl_draw(getStr(), 3, 3, w() - 6, h() - 6,
            Fl_Align(FL_ALIGN_LEFT | FL_ALIGN_WRAP));
}

void TipWin::showValue(float f)
{
    //convert the value to a string
    char tmp[10];
    snprintf(tmp, 9, "%.2f", f);
    tip = tmp;

    textmode = false;
    redraw();
    show();
}

void TipWin::setText(const char *c)
{
    text     = c;
    textmode = true;
    redraw();
}

void TipWin::showText()
{
    if(!text.empty()) {
        textmode = true;
        redraw();
        show();
    }
}

void TipWin::redraw()
{
    // Recalc size of window
    fl_font(labelfont(), labelsize());
    int W = 0, H = 0;
    fl_measure(getStr(), W, H, 0);
    //provide a bit of extra space
    W += 8;
    H += 4;
    size(W, H);
    Fl_Menu_Window::redraw();
}

const char *TipWin::getStr() const
{
    return (textmode ? text : tip).c_str();
}

//static int numobj = 0;

WidgetPDial::WidgetPDial(int x, int y, int w, int h, const char *label)
    :Fl_Dial(x, y, w, h, label), oldvalue(0.0f), pos(false), textset(false)
{
    //cout << "[" << label << "] There are now " << ++numobj << endl;
    Fl_Group *save = Fl_Group::current();
    tipwin = new TipWin();
    tipwin->hide();
    Fl_Group::current(save);
}

WidgetPDial::~WidgetPDial()
{
    //cout << "There are now " << --numobj << endl;
    delete tipwin;
}

int WidgetPDial::handle(int event)
{
//#ifdef NTK_GUI
//    return Fl_Dial::handle( event );
//#else
    double dragsize, min = minimum(), max = maximum();
    int    my;

    switch(event) {
        case FL_PUSH:
            oldvalue = value();
        case FL_DRAG:
            getPos();
            tipwin->showValue(value());
            my = -(Fl::event_y() - y() - h() / 2);

            dragsize = 200.0f;
            if(Fl::event_state(FL_BUTTON1) == 0)
                dragsize *= 10;

            value(limit(oldvalue + my / dragsize * (max - min), min, max));
            value_damage();
            if(this->when() != 0)
                do_callback();
            return 1;
        case FL_ENTER:
            getPos();
            tipwin->showText();
            return 1;
        case FL_HIDE:
        case FL_LEAVE:
            tipwin->hide();
            resetPos();
            break;
        case FL_RELEASE:
            tipwin->hide();
            resetPos();
            if(this->when() == 0)
                do_callback();
            return 1;
            break;
    }
    return 0;
//#endif
}

void WidgetPDial::drawgradient(int cx, int cy, int sx, double m1, double m2)
{
#ifdef NTK_GUI
    return;
#else
    for(int i = (int)(m1 * sx); i < (int)(m2 * sx); i++) {
        double tmp = 1.0f - powf(i * 1.0f / sx, 2.0f);
        pdialcolor(140
                   + (int) (tmp
                            * 90), 140
                   + (int)(tmp * 90), 140 + (int) (tmp * 100));
        fl_arc(cx + sx / 2 - i / 2, cy + sx / 2 - i / 2, i, i, 0, 360 );
    }
#endif
}

void WidgetPDial::draw()
{
#ifdef NTK_GUI
    box( FL_NO_BOX );

    Fl_Dial::draw();
    
    return;
#else
    int cx = x(), cy = y(), sx = w(), sy = h();

    //clears the button face
    pdialcolor(190, 190, 200);
    fl_pie(cx - 1, cy - 1, sx + 2, sy + 2, 0, 360);

    /* //Draws the button face (gradinet) */
    drawgradient(cx, cy, sx, 0.5f, 1.0f); 

    double val = (value() - minimum()) / (maximum() - minimum());

    //draws the scale
    pdialcolor(220, 220, 250);
    double a1 = angle1(), a2 = angle2();
    for(int i = 0; i < 12; i++) {
        double a = -i / 12.0f * 360.0f - val * (a2 - a1) - a1;
        fl_pie(cx, cy, sx, sy, a + 270 - 3, a + 3 + 270);
    }

    drawgradient(cx, cy, sx, 0.0f, 0.75f);



    //draws the value
    double a = -(a2 - a1) * val - a1;

    //draws the max and min points
    pdialcolor(0, 100, 200);
    int xp =
        (int)(cx + sx / 2.0f + sx / 2.0f * sinf(angle1() / 180.0f * 3.141592f));
    int yp =
        (int)(cy + sy / 2.0f + sy / 2.0f * cosf(angle1() / 180.0f * 3.141592f));
    fl_pie(xp - 2, yp - 2, 4, 4, 0, 360);

    xp = (int)(cx + sx / 2.0f + sx / 2.0f * sinf(angle2() / 180.0f * 3.141592f));
    yp = (int)(cy + sy / 2.0f + sy / 2.0f * cosf(angle2() / 180.0f * 3.141592f));
    fl_pie(xp - 2, yp - 2, 4, 4, 0, 360);

    fl_push_matrix();

    fl_translate(cx + sx / 2, cy + sy / 2);
    fl_rotate(a - 90.0f);

    fl_translate(sx / 2, 0);

    fl_begin_polygon();
    pdialcolor(0, 0, 0);
    fl_vertex(-10, -4);
    fl_vertex(-10, 4);
    fl_vertex(0, 0);
    fl_end_polygon();

    fl_pop_matrix();
#endif
}

void WidgetPDial::pdialcolor(int r, int g, int b)
{
    if(active_r())
        fl_color(r, g, b);
    else
        fl_color(160 - (160 - r) / 3, 160 - (160 - b) / 3, 160 - (160 - b) / 3);
}

void WidgetPDial::tooltip(const char *c)
{
    tipwin->setText(c);
    textset = true;
}

void WidgetPDial::getPos()
{
    if(!pos) {
        tipwin->position(Fl::event_x_root(), Fl::event_y_root() + 20);
        pos = true;
    }
}

void WidgetPDial::resetPos()
{
    pos = false;
}
