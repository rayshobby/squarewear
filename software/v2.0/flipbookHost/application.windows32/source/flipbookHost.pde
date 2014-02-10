import g4p_controls.*;
import com.codeminders.hidapi.HIDDeviceInfo;
import com.codeminders.hidapi.HIDManager;
import com.codeminders.hidapi.HIDDevice;
import java.util.List;
import java.awt.Rectangle;

// HID device VID/PID
int VENDOR_ID = 0x16c0;
int PRODUCT_ID = 0x05df;
HIDDevice device = null;
boolean device_initialized = false;

boolean DEBUG = true; 

int nBoardX = 1;  // number of boards in the x direction
int nBoardY = 1;  // number of boards in the y direction
int nFrames = 1;  // number of animation frames
//int nBright = 128; 

color[][][] colors;      // stores pixel colors of all frames
int[] times;             // stores frame times
int[] tones;             // stores frame tones
int currFrame = 0;

int DEFAULT_FRAMETIME = 100;

// GUI related parameters and variables
int DEFAULT_PIXELSIZE = 80;
int DEFAULT_PIXELGAP = 5;
int PIXELSIZE = DEFAULT_PIXELSIZE;
int PIXELGAP =  DEFAULT_PIXELGAP;
int PIXELGRID_STARTX = 10;
int PIXELGRID_STARTY = 110;

int FRAME_CONFIG_STARTX = 0;
int FRAME_CONFIG_STARTY = 0;

int CTRL_PANEL_STARTX = 440;
int CTRL_PANEL_STARTY = 380;

int COLOR_PANEL_STARTX = 440;
int COLOR_PANEL_STARTY = 70;

GButton btnSaveAnimation;
GButton btnLoadAnimation;
GButton btnConnect;
GButton btnFillPixels;
GButton btnSaveToDevice;
GButton btnLoadFromDevice;
GButton btnAddFrame;
GButton btnNextFrame;
GButton btnPrevFrame;
GButton btnDelFrame;

GSlider sliderXBoards;
GSlider sliderYBoards;
GTextArea textFrameInfo;
GTextField inputFrameTime;
GDropList droplistFrameTone;
GLabel lblFrameTime;
GLabel lblXSlider;
GLabel lblYSlider;
GLabel lblMouseClick;
GLabel lblFrameInfo;
GLabel lblFrameTone;

Rectangle lcPicker, rcPicker, mcPicker;

boolean guiElementClicked = false;
boolean pixel_selected = false;
int picker_selected = 0;
boolean fill_selected = false;
int xToSet;
int yToSet;
float time = 0;

color lcColor = color(255, 0, 0);  // background color:
color rcColor = color(0, 0, 0);    // right click color
color mcColor = color(0, 255, 0);  // middle click color
color tempLCColor = lcColor;

PImage imageColorSelector = null;

public void setup() {
  size(610, 720);
  G4P.setGlobalColorScheme(5);
  colors = new color[5][7][1];
  times = new int[1];
  tones = new int[1];
  times[0] = DEFAULT_FRAMETIME;
  
  // control panel 
  btnConnect = new GButton(this,CTRL_PANEL_STARTX+10,CTRL_PANEL_STARTY+15,130,25,"Connect");
  btnConnect.setTextBold();
  btnSaveAnimation = new GButton(this,CTRL_PANEL_STARTX+10,CTRL_PANEL_STARTY+50,130,25, "Save to File");
  btnSaveAnimation.setTextBold();
  btnLoadAnimation = new GButton(this,CTRL_PANEL_STARTX+10,CTRL_PANEL_STARTY+85,130,25, "Load from File");
  btnLoadAnimation.setTextBold();
  btnFillPixels = new GButton(this,CTRL_PANEL_STARTX+10,CTRL_PANEL_STARTY+120,130,25,"Fill Pixels");
  btnFillPixels.setTextBold();
  btnSaveToDevice = new GButton(this,CTRL_PANEL_STARTX+10,CTRL_PANEL_STARTY+155,130,25, "Save to Device");
  btnSaveToDevice.setTextBold();
  btnLoadFromDevice = new GButton(this,CTRL_PANEL_STARTX+10,CTRL_PANEL_STARTY+190,130,25,"Load from Device");
  btnLoadFromDevice.setTextBold();

  // x, y boards sliders
  sliderXBoards = new GSlider(this,CTRL_PANEL_STARTX+10,CTRL_PANEL_STARTY+245,130,20,10);
  sliderXBoards.setLimits(1,1,4);
  sliderXBoards.setNumberFormat(G4P.INTEGER);
  sliderXBoards.setNbrTicks(4);
  sliderXBoards.setStickToTicks(true);
  lblXSlider = new GLabel(this, CTRL_PANEL_STARTX+10, CTRL_PANEL_STARTY+227, 130, 20, "#. Boards in X");
  lblXSlider.setLocalColorScheme(7);
  lblXSlider.setTextBold();
  
  sliderYBoards = new GSlider(this,CTRL_PANEL_STARTX+10,CTRL_PANEL_STARTY+285,130,20,10);
  sliderYBoards.setLimits(1,1,4);
  sliderYBoards.setNumberFormat(G4P.INTEGER);
  sliderYBoards.setNbrTicks(4);
  sliderYBoards.setStickToTicks(true);
  lblYSlider = new GLabel(this, CTRL_PANEL_STARTX+10, CTRL_PANEL_STARTY+267, 130, 20, "#. Boards in Y");
  lblYSlider.setLocalColorScheme(7);
  lblYSlider.setTextBold();  
  
  // color selector panel
  lblMouseClick = new GLabel(this, COLOR_PANEL_STARTX+5,COLOR_PANEL_STARTY+245, 135, 20, "Mouse Click Colors:");
  lblMouseClick.setLocalColorScheme(7);
  lblMouseClick.setTextBold();
  
  btnPrevFrame = new GButton(this,FRAME_CONFIG_STARTX+20,FRAME_CONFIG_STARTY+18,100,24,"< prev frame");
  btnPrevFrame.setTextBold();
  btnNextFrame = new GButton(this,FRAME_CONFIG_STARTX+260,FRAME_CONFIG_STARTY+18,100,24,"next frame >");
  btnNextFrame.setTextBold();  
  btnAddFrame = new GButton(this,FRAME_CONFIG_STARTX+130,FRAME_CONFIG_STARTY+5,120,50,"Add a\nNew Frame");
  btnAddFrame.setTextBold();
  btnDelFrame = new GButton(this,FRAME_CONFIG_STARTX+520,FRAME_CONFIG_STARTY+5,60,50, "Delete This Frame");
  btnDelFrame.setTextBold();
  
  lcPicker = new Rectangle(COLOR_PANEL_STARTX+10,COLOR_PANEL_STARTY+265,45,45);
  mcPicker = new Rectangle(COLOR_PANEL_STARTX+60,COLOR_PANEL_STARTY+265,45,45);
  rcPicker = new Rectangle(COLOR_PANEL_STARTX+110,COLOR_PANEL_STARTY+265,45,45);
  
  // frame configuration panel
  inputFrameTime = new GTextField(this,FRAME_CONFIG_STARTX+130,FRAME_CONFIG_STARTY+68,50,25);
  
  lblFrameTime = new GLabel(this, FRAME_CONFIG_STARTX+10, FRAME_CONFIG_STARTY+70, 120, 20, "Frame Time (ms):");
  lblFrameTime.setLocalColorScheme(7);
  lblFrameTime.setTextBold();
  
  lblFrameInfo = new GLabel(this, FRAME_CONFIG_STARTX+360,FRAME_CONFIG_STARTY+20, 150,25);
  lblFrameInfo.setLocalColorScheme(7);
  
  lblFrameTone = new GLabel(this, FRAME_CONFIG_STARTX+220, FRAME_CONFIG_STARTY+70, 120, 20, "Frame Tone:");
  lblFrameTone.setLocalColorScheme(7);
  lblFrameTone.setTextBold();
  
  droplistFrameTone = new GDropList(this,FRAME_CONFIG_STARTX+330,FRAME_CONFIG_STARTY+65,35,300,10);
  droplistFrameTone.setItems(pitchesArray.pitches,0);
  droplistFrameTone.setSelected(0);

  enableGUI(false, false);
  imageColorSelector = loadImage("colorSelector.png"); 
}

public void enableGUI(boolean en, boolean all){
  btnSaveAnimation.setEnabled(en);
  btnLoadAnimation.setEnabled(en);
  btnFillPixels.setEnabled(en);
  btnAddFrame.setEnabled(en);
  btnDelFrame.setEnabled(en);
  btnNextFrame.setEnabled(en);
  btnPrevFrame.setEnabled(en);
  btnSaveToDevice.setEnabled(en);
  btnLoadFromDevice.setEnabled(en);
  droplistFrameTone.setEnabled(en);
  inputFrameTime.setEnabled(en);
  if (all) {
    sliderXBoards.setEnabled(en);
    sliderYBoards.setEnabled(en);
    btnConnect.setEnabled(en);
  }
}

public void renderPicker(Rectangle r, color c, String t, int idx) {
  if (picker_selected == idx) {
    fill(compColor(c));
    rect(r.x-2, r.y-2, r.width+4, r.height+4);
  }
  fill(c);
  rect(r.x, r.y, r.width, r.height);
  fill(compColor(c));
  text(t, r.x + r.width/2-5, r.y + r.height/2+5);
}

public void draw() {
  background(32,64,32);    // background color: dark green
  
  if(device == null){
      lblFrameInfo.setText("Frame: -- of --");
      lblFrameInfo.setTextBold();
  } else {
      lblFrameInfo.setText("Frame: "+(currFrame+1) + " of " + nFrames);
      lblFrameInfo.setTextBold();
  }
  
  // render color selection panel
  image(imageColorSelector,COLOR_PANEL_STARTX,COLOR_PANEL_STARTY,imageColorSelector.width,imageColorSelector.height);

  noStroke();
  textSize(14);

  renderPicker(lcPicker, tempLCColor, "L", 1);
  renderPicker(mcPicker, mcColor, "M", 2);
  renderPicker(rcPicker, rcColor, "R", 3);

  // render pixel grid
  for(int i = 0; i < 5*nBoardX; i++){
    for(int j = 0; j < 7*nBoardY; j++){
      if((pixel_selected && xToSet == i && yToSet == j) || fill_selected){
        fill(compColor(colors[i][j][currFrame]));
        ellipse(PIXELGRID_STARTX+i*(PIXELSIZE+PIXELGAP)+PIXELSIZE/2, PIXELGRID_STARTY+j*(PIXELSIZE+PIXELGAP)+PIXELSIZE/2, PIXELSIZE+2, PIXELSIZE+2);
      } /*else if(colors[i][j][currFrame] == color(0,0,0) || colors[i][j][currFrame] == color(255,255,255) || colors[i][j][currFrame] == 0){
        fill(128,128,128);
        ellipse(i*(PIXELSIZE+PIXELGAP)+11+PIXELSIZE/2,j*(PIXELSIZE+PIXELGAP)+11 + PIXELSIZE/2,PIXELSIZE+4,PIXELSIZE+4);
      }  */
      fill(colors[i][j][currFrame]);
      ellipse(PIXELGRID_STARTX+i*(PIXELSIZE+PIXELGAP)+PIXELSIZE/2, PIXELGRID_STARTY+j*(PIXELSIZE+PIXELGAP)+PIXELSIZE/2, PIXELSIZE, PIXELSIZE);
    }
  }
  if(pixel_selected || fill_selected){
    if(!isOverPixel(xToSet,yToSet) && !lcPicker.contains(mouseX, mouseY)){
      tempLCColor = (get(mouseX,mouseY) | 0xFF000000);
    } else {
      tempLCColor = lcColor;
    }
  }  
}

/*
public void mouseDragged() {
  if(device == null) {return;}
  if(picker_selected != 0) {
    if (mouseButton == LEFT) {
      lcColor = get(mouseX, mouseY);
    } else if (mouseButton == RIGHT) {
      rcColor = get(mouseX, mouseY);
    } else if (mouseButton == CENTER) {
      mcColor = get(mouseX, mouseY);
    }
  }  
}*/

public void mouseClicked() {
  if(millis()-time < 50){return;}
  if(guiElementClicked) {
    guiElementClicked = false;
    return;
  }
  if(device == null){return;}
  if(!(pixel_selected || fill_selected || picker_selected > 0)){
    for(int i = 0; i < 5*nBoardX; i++){
      for(int j = 0; j < 7*nBoardY; j++){
        if(isOverPixel(i,j)){
         xToSet = i;
         yToSet = j;
         pixel_selected = true;
         enableGUI(false, true);
         return;
        }
      }
    }
    if (lcPicker.contains(mouseX, mouseY)) {
      picker_selected = 1;
    } else if (mcPicker.contains(mouseX, mouseY)) {
      picker_selected = 2;
    } else if (rcPicker.contains(mouseX, mouseY)) {
      picker_selected = 3;
    }
    if (picker_selected != 0) {
      enableGUI(false, true);
      return;
    }
    /*
    if(COLOR_PANEL_STARTX < mouseX && mouseX < COLOR_PANEL_STARTX + imageColorSelector.width &&
       COLOR_PANEL_STARTY < mouseY && mouseY < COLOR_PANEL_STARTY + imageColorSelector.height) {
      if (mouseButton == LEFT) {
        lcColor = get(mouseX, mouseY);
      } else if (mouseButton == RIGHT) {
        rcColor = get(mouseX, mouseY);
      } else if (mouseButton == CENTER) {
        mcColor = get(mouseX, mouseY);
      }
    }  */    
    return;
  }
  if(picker_selected != 0) {
    color c = get(mouseX, mouseY);
    c = color((c>>16)&0xFF, (c>>8)&0xFF, c&0xFF);
    if(picker_selected == 1){lcColor = c;tempLCColor = lcColor;}
    else if(picker_selected == 2) mcColor = c;
    else rcColor = c;
    picker_selected = 0;
    enableGUI(true, true);
    return; 
  }
  if(pixel_selected) {
    /*color c = get(mouseX, mouseY);
    int r = ((c & 0xFF0000) >> 16) >> 1;
    int g = ((c & 0x00FF00) >> 8 ) >> 1;
    int b = ((c & 0x0000FF)) >> 1;
    c = 0xFF000000 | (r << 16) | (g << 8) | b;*/
    if (mouseButton == LEFT) {
      colors[xToSet][yToSet][currFrame] = tempLCColor;
      tempLCColor = lcColor;
    } else if (mouseButton == RIGHT) {
      colors[xToSet][yToSet][currFrame] = rcColor;
    } else {
      colors[xToSet][yToSet][currFrame] = mcColor;
    }
    updatePixel(xToSet,yToSet,colors[xToSet][yToSet][currFrame]);
    pixel_selected = false;
    enableGUI(true, true);
    return;
  }
  if(fill_selected){
    color c;
    /*color c = get(mouseX, mouseY);
    int r = ((c & 0xFF0000) >> 16) >> 1;
    int g = ((c & 0x00FF00) >> 8 ) >> 1;
    int b = ((c & 0x0000FF)) >> 1;
    c = 0xFF000000 | (r << 16) | (g << 8) | b;*/
    if (mouseButton == LEFT) {
      c = lcColor;
    } else if (mouseButton == RIGHT) {
      c = rcColor;
    } else {
      c = mcColor;
    }    
    fillPixels(c);
    fill_selected = false;
    enableGUI(true, true);
    return;
  }
}

public void fillPixels(color c){
  for(int i = 0; i < 5*nBoardX; i++){
    for(int j = 0; j < 7*nBoardY; j++){
      colors[i][j][currFrame] = c;
    }
  }
  sync();
}

public void updatePixel(int i, int j, color c){
  int x = '0' + i;
  int y = '0' + j;
  int r = (c >> 16) & 0xFF;
  int g = (c >> 8 ) & 0xFF;
  int b = (c      ) & 0xFF;
  r = (int)(pow((r / 512.f), 2.2f) *255);
  g = (int)(pow((g / 512.f), 2.2f) *255);
  b = (int)(pow((b / 512.f), 2.2f) *255);
  String s = "" + (char)x + (char)y + (char)(r/16 + '0') + (char)(r%16 + '0') + (char)(g/16 + '0') + (char)(g%16 + '0') + (char)(b/16 + '0') + (char)(b%16 + '0');
  if (DEBUG)
    System.out.println(s);
  for(int k = 0; k < 5; k++){  // send multiple times, simple hack to improve robustness
    send(s);
  }
}

public void handleButtonEvents(GButton button, GEvent event){
  guiElementClicked = true;
  if(button == btnSaveAnimation && event == GEvent.CLICKED){
    String loc = null;
    loc = g4p_controls.G4P.selectOutput("Select Location","txt","text file");
    if(loc == null){return;}
    saveAnimationToFile(loc);
    return;
  }
  if(button == btnLoadAnimation && event == GEvent.CLICKED){
    String loc = null;
    loc = g4p_controls.G4P.selectInput("select file", "txt","text file");
    if(loc == null){return;}
    loadAnimationFromFile(loc);
    return;   
  }
  if(button == btnFillPixels){
    fill_selected = true;
    enableGUI(false, true);
    return;
  }
  if(button == btnConnect){
    if(device == null){
      deviceFindFirst();
      if(device != null){
        btnConnect.setText("Disconnect");
        btnConnect.setTextBold();
        /*nFrames = 1;
        nBoardX = 1;
        nBoardY = 1;*/
        nFrames = 1;
        currFrame = 0;
        colors = new color[nBoardX*5][nBoardY*7][nFrames];
        times = new int[1];
        tones = new int[1];
        
        times[0] = DEFAULT_FRAMETIME;
        inputFrameTime.setText(""+times[0]);
        inputFrameTime.setTextBold();
        droplistFrameTone.setSelected(tones[0]);
        
        fillPixels(color(0,0,0));
        droplistFrameTone.setSelected(0);
        enableGUI(true, false);
      }
    } else {
      fillPixels(color(0,0,0));
      try{
        device.close();
      } catch(IOException ioe) {
        ioe.printStackTrace();
      }
      device = null;
      btnConnect.setText("Connect");
      btnConnect.setTextBold();
      enableGUI(false, false);
    }
    return;
  }
  if(button == btnSaveToDevice){
    enableGUI(false, true);
    for(int i = 0; i < nFrames; i++){
      currFrame = i;
      sync();
      delay(750);
      send("x");
      delay(5*35*nBoardX*nBoardY*3+100);
    }
    time = millis();
    enableGUI(true, true);
  }
  if(button == btnLoadFromDevice){
    enableGUI(false, true);
    send("l");
    String s = null;
    String s2 = null;
    while(s == null){
      s = deviceRead();
    }
    if (DEBUG)
      System.out.println(s);
    nFrames = s.charAt(0) - '0';
    nBoardX = s.charAt(1) - '0';
    nBoardY = s.charAt(2) - '0';
    //nBright = s.charAt(3) - '0';
    
    //update pixel size
     PIXELSIZE = Math.min((DEFAULT_PIXELSIZE+DEFAULT_PIXELGAP)/nBoardX-DEFAULT_PIXELGAP,
                         (DEFAULT_PIXELSIZE+DEFAULT_PIXELGAP)/nBoardY-DEFAULT_PIXELGAP);
    
    colors = new color[nBoardX*5][nBoardY*7][nFrames];
    times = new int[nFrames];
    tones = new int[nFrames];
    s = null;
    for(int i = 0; i < nBoardX*nBoardY*35*nFrames; i++){
      while(s == null || s.length() != 8){s = deviceRead();}
      while(s2 == null || s2.length() != 1){s2 = deviceRead();}
      s += s2;
      if (DEBUG)
        System.out.println(s); 
      int f = s.charAt(0) - '0';
      int x = s.charAt(1) - '0';
      int y = s.charAt(2) - '0';
      int r = (s.charAt(3) - '0')*16 + (s.charAt(4) - '0');
      int g = (s.charAt(5) - '0')*16 + (s.charAt(6) - '0');
      int b = (s.charAt(7) - '0')*16 + (s.charAt(8) - '0');
      r = (int)(pow((r / 255.f), 1/2.2f) * 512);
      if (r>255) r=255;
      g = (int)(pow((g / 255.f), 1/2.2f) * 512);
      if (g>255) g=255;
      b = (int)(pow((b / 255.f), 1/2.2f) * 512);
      if (b>255) b=255;      
      colors[x][y][f] = color(r,g,b);
      currFrame = f;
      s = null;
      s2 = null;
    }
    for(int i = 0; i < nFrames; i++){
      while(s == null){s = deviceRead();}
      times[s.charAt(0)-'0'] = 256*(s.charAt(1)-'0')+16*(s.charAt(2)-'0')+(s.charAt(3)-'0');
      tones[s.charAt(0)-'0'] = 16*(s.charAt(4)-'0')+(s.charAt(5)-'0');
      s = null;    
    }
    sync();
    inputFrameTime.setText(""+times[currFrame]);
    inputFrameTime.setTextBold();
    droplistFrameTone.setSelected(tones[currFrame]);
    time = millis();
    enableGUI(true, true);
  }
  /*if(button == chooseNote){
    toggleButtons();
    droplistFrameTone.setVisible(true);
  }*/
  if(button == btnNextFrame){
    currFrame = (currFrame+1)%nFrames;
    sync();
  }
  if(button == btnPrevFrame){
    currFrame = (currFrame+nFrames-1)%nFrames;
    sync();
  }
  if(button == btnAddFrame){
    color[][][] temp = colors;
    nFrames++;
    currFrame = nFrames - 1;
    colors = new color[nBoardX*5][nBoardY*7][nFrames];
    for(int i = 0; i < nBoardX*5; i++){
      for(int j = 0 ; j < nBoardY*7; j++){
        for(int k = 0; k < nFrames-1; k++){
          colors[i][j][k] = temp[i][j][k];
        }
      }
    }
    int[] tempT = times;
    times = new int[nFrames];
    for(int i = 0; i < nFrames-1; i++){
      times[i] = tempT[i];
    }
    times[nFrames-1] = DEFAULT_FRAMETIME;
    tempT = tones;
    tones = new int[nFrames];
    for(int i = 0; i < nFrames-1; i++){
      tones[i] = tempT[i];
    }
    sync();
  }
  if(button == btnDelFrame){
    if(nFrames > 1){
      color[][][] temp = colors;
      nFrames--;
      colors = new color[nBoardX*5][nBoardY*7][nFrames];
      for(int f = 0; f < currFrame; f++){
        for(int i = 0; i < 5*nBoardX; i++){
          for(int j = 0; j < 7*nBoardY; j++){
            colors[i][j][f] = temp[i][j][f];
          }
        }
      }
      for(int f = currFrame+1; f < nFrames+1; f++){
        for(int i = 0; i < 5*nBoardX; i++){
          for(int j = 0; j < 7*nBoardY; j++){
            colors[i][j][f-1] = temp[i][j][f];
          }
        }
      }
      currFrame = Math.min(currFrame,nFrames-1);
      int[] tempT = times;
      times = new int[nFrames];
      for(int i = 0; i < currFrame; i++){
        times[i] = tempT[i];
      }
      for(int i = currFrame+1; i < nFrames+1; i++){
        times[i-1] = tempT[i];
      }
      tempT = tones;
      tones = new int[nFrames];
      for(int i = 0; i < currFrame; i++){
        tones[i] = tempT[i];
      }
      for(int i = currFrame+1; i < nFrames+1; i++){
        tones[i-1] = tempT[i];
      }
      sync();
    }
  }
}

public void handleSliderEvents(GValueControl slider,GEvent event){
  if(slider == sliderXBoards || slider == sliderYBoards){
    int oldX = nBoardX;
    int oldY = nBoardY;
    if(slider == sliderXBoards){
      nBoardX = slider.getValueI();
      if(nBoardX * nBoardY > 12){
        nBoardY--;
        sliderYBoards.setValue(nBoardY);
      }
    }
    if(slider == sliderYBoards){
      nBoardY = slider.getValueI();
      if(nBoardX * nBoardY > 12){
        nBoardX--;
        sliderXBoards.setValue(nBoardX);
      }
    }
    PIXELSIZE = Math.min((DEFAULT_PIXELSIZE+DEFAULT_PIXELGAP)/nBoardX-DEFAULT_PIXELGAP,
                         (DEFAULT_PIXELSIZE+DEFAULT_PIXELGAP)/nBoardY-DEFAULT_PIXELGAP);
    color[][][] temp = colors;
    colors = new color[5*nBoardX][7*nBoardY][nFrames];
    for(int i = 0; i < 5*Math.min(oldX,nBoardX); i++){
      for(int j = 0; j < 7*Math.min(oldY,nBoardY); j++){
        for(int k = 0; k < nFrames; k++){
          colors[i][j][k] = temp[i][j][k];
        }
      }
    }
    send("" + (char)(nFrames+'0') + nBoardX + nBoardY + (char)(currFrame+'0'));
  }
  /*if(slider == brightness){
    nBright = slider.getValueI();
    slid = true;
  }*/
}

public void handleTextEvents(GEditableTextControl textarea, GEvent event) {
  if(textarea.equals(inputFrameTime) && event == GEvent.ENTERED ) {
    try{
      int t = Integer.parseInt(inputFrameTime.getText());
      times[currFrame] = t;
      inputFrameTime.setTextBold();
      inputFrameTime.setFocus(false);
    } catch (NumberFormatException nfe){
    }
  } 
}

public void handleDropListEvents(GDropList list, GEvent event){
  guiElementClicked = true;
  if(list == droplistFrameTone){
    tones[currFrame] = droplistFrameTone.getSelectedIndex();
    if (DEBUG)
      System.out.println(tones[currFrame]);
    //droplistFrameTone.setVisible(false);
    send("" + (char)(times[currFrame]/256+'0') + (char)(times[currFrame]%256/16+'0') + (char)(times[currFrame]%16+'0') + (char)(tones[currFrame]/16+'0') + (char)(tones[currFrame]%16+'0'));
    //enableGUI(false);
  }
}

void keyPressed() {
  if(device != null){
    if (key == ESC) {
      if (fill_selected) { fill_selected = false; }
      if (pixel_selected) { pixel_selected = false;}
      key = 0;
    }
    if(key == CODED){
      if(keyCode == RIGHT){
        currFrame = (currFrame+1)%nFrames;
        sync();
      }
      if(keyCode == LEFT){
        currFrame = (currFrame+nFrames-1)%nFrames;
        sync();
      }
    }
    if(key == 'f'){
      fill_selected = true;
      enableGUI(false, true);
    }
    if(key == 's'){
      String loc = null;
      loc = g4p_controls.G4P.selectOutput("Select Location","txt","text file");
      if(loc == null){return;}
      saveAnimationToFile(loc);
    }
    if(key == 'l'){
      String loc = null;
      loc = g4p_controls.G4P.selectOutput("Select Location","txt","text file");
      if(loc == null){return;}
      loadAnimationFromFile(loc);
    }
  }
}

public void sync(){
  if (DEBUG)
    System.out.println("sync");
  for(int i = 0; i < nBoardX*5; i++){
    for(int j = 0; j < nBoardY*7; j++){
      updatePixel(i,j,colors[i][j][currFrame]);
    }
  }
  inputFrameTime.setText(""+times[currFrame]);
  inputFrameTime.setTextBold();
  droplistFrameTone.setSelected(tones[currFrame]);
  send("" + (char)(nFrames+'0') + nBoardX + nBoardY + (char)(currFrame+'0'));
  send("" + (char)(times[currFrame]/256+'0') + (char)(times[currFrame]%256/16+'0') + (char)(times[currFrame]%16+'0') + (char)(tones[currFrame]/16+'0') + (char)(tones[currFrame]%16+'0'));
  delay(times[currFrame]+500);
  send(""  + (char)(255/16+'0') + (char)(255%16 + '0'));
  delay(500);
}

// send a string to device
private void send(String s) {
  if( device != null ) {
    if( s != null && !s.equals("") ) {
      deviceWrite( s );
    }
  }
}

public void deviceWrite(String value) {
  char[] charArray = value.toCharArray();
  int size = (charArray.length > 32) ? 32 : charArray.length;
  byte[] data = new byte[33];
  data[0] = (byte)0;
  int i;
  for(i=0;i<size;i++) {
    data[i+1]=(byte)charArray[i];
  }
  data[i+1]=0;
  try {
    device.sendFeatureReport(data);
    //delay(250);
  } catch(Exception e) {
//    e.printStackTrace();
//    System.out.println("deviceWrite error");
  }
}


public void deviceInitialize() {
  if (!device_initialized) {
    device_initialized = true;
    com.codeminders.hidapi.ClassPathLibraryLoader.loadNativeHIDLibrary();
  }
}

public void deviceFindFirst() {
  deviceInitialize();
  HIDDeviceInfo[] infos = deviceFindAllDescriptors();

  if (infos.length > 0) {
    try {
      device = infos[0].open();
    } catch (Exception e) {
      device = null;
    }
  }  
}

public HIDDeviceInfo[] deviceFindAllDescriptors() {
  deviceInitialize();
    
  List<HIDDeviceInfo> devlist = new ArrayList<HIDDeviceInfo>();

  try {
    HIDManager hidManager = HIDManager.getInstance();

    HIDDeviceInfo[] infos = hidManager.listDevices();

    for (HIDDeviceInfo info : infos) {
      if (info.getVendor_id() == VENDOR_ID
          && info.getProduct_id() == PRODUCT_ID) {
        devlist.add(info);
      }
    }
  } catch (Exception e) {
  
  }
  return devlist.toArray(new HIDDeviceInfo[devlist.size()]);  
}

public String deviceRead() {
  try {
    device.disableBlocking();
    byte[] data = new byte[10];
    int read = device.read(data);
    if (read > 0) {
      String str = new String();
      for(int i=0;i<read;i++) {
        if(data[i]!=0) str+=((char)data[i]);
      }
      return str;
    } else {
      return null;
    }
  } catch(IOException ioe) {
    //ioe.printStackTrace();
    if (DEBUG)
      System.out.println("deviceRead error");
  }
  return null;
}

// delay for dd milliseconds
void delay(int dd) {
  long t = millis();
  while(millis() <= dd + t);
}

// compute complement color
color compColor(color c) {
  int r = (c >> 16) & 0xFF;
  int g = (c >> 8) & 0xFF;
  int b = c & 0xFF;
  return color(255-r, 255-g, 255-b);
}  

// check if mouse (x, y) falls inside a pixel
boolean isOverPixel(int i, int j) {
  int xDist = mouseX - (PIXELGRID_STARTX+i*(PIXELSIZE+PIXELGAP) + PIXELSIZE/2);
  int yDist = mouseY - (PIXELGRID_STARTY+j*(PIXELSIZE+PIXELGAP) + PIXELSIZE/2);
  if(xDist*xDist+yDist*yDist <= (PIXELSIZE*PIXELSIZE)/4){
    return true;  
  }
  return false;
}

void saveAnimationToFile(String file){
  String[] lines = new String[1+8*nFrames];
  
  lines[0] = "" + nFrames + "," + nBoardX + "," + nBoardY;
  for(int i = 0; i < nFrames; i++){
    lines[8*i+1] = "" + times[i] + "," + tones[i];
    for(int j = 0; j < 7; j++){
      String s = "";
      for(int k = 0; k < 5*nBoardX; k++){
        s += "0x" + hex(colors[k][j][i]);
        if(k != 5*nBoardX-1){s += ",";}
      }
      lines[8*i+2+j] = s;
    }
  }
  saveStrings(file,lines);   
}
void loadAnimationFromFile(String file){
  String[] curLine;
  String[] allLines;
  
  allLines = loadStrings(file);

  curLine = split(allLines[0],',');
  
  nFrames = new Integer(curLine[0]);
  nBoardX = new Integer(curLine[1]);
  nBoardY = new Integer(curLine[2]);
  
  colors = new color[5*nBoardX][7*nBoardY][nFrames];
  times = new int[nFrames];
  tones = new int[nFrames];
  
  //update pixel size
  PIXELSIZE = Math.min((DEFAULT_PIXELSIZE+DEFAULT_PIXELGAP)/nBoardX-DEFAULT_PIXELGAP,
                         (DEFAULT_PIXELSIZE+DEFAULT_PIXELGAP)/nBoardY-DEFAULT_PIXELGAP);

  
  for(int i = 0; i < nFrames; i++){
    curLine = split(allLines[i*8+1],',');
    times[i] = new Integer(curLine[0]);
    tones[i] = new Integer(curLine[1]);
    for(int j = 0; j < 7*nBoardY; j++){
      curLine = split(allLines[i*8+j+2],',');
      for(int k = 0; k < 5*nBoardX; k++){
        int p = (int)(Long.parseLong(curLine[k].substring(2),16));
        colors[k][j][i] = p;
      }
    }
  }
  sync();
}
