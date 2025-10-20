#include "drawingcanvas.h"

DrawingCanvas::DrawingCanvas(QWidget *parent)  {
    // Set a minimum size for the canvas
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    // Set a solid background color
    setStyleSheet("background-color: white; border: 1px solid gray;");
}

void DrawingCanvas::clearPoints(){
    m_points.clear();
    // Trigger a repaint to clear the canvas
    m_endpointPixels.clear();
    m_cornerPixels.clear();
    m_bodyMiddlePixels.clear();
    m_intersectionPixels.clear();
    m_detectionVisualizationActive = false;
    update();
}

void DrawingCanvas::paintLines(){
    /* Todo
     * Implement lines drawing per even pair
    */

    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::segmentDetection(){
    // m_endpointPixels.clear();
    // m_cornerPixels.clear();
    // m_bodyMiddlePixels.clear();
    // m_intersectionPixels.clear();

    QPixmap pixmap = this->grab(); //
    QImage image = pixmap.toImage();

    cout << "image width " << image.width() << endl;
    cout << "image height " << image.height() << endl;

    cout << "Analyzing image (" << image.width() << "x" << image.height() << ")..." << endl;

    // Iterate through each pixel, classify its 3x3 window, and store its location.
    // This is much more efficient than creating large intermediate vectors.
    for(int i = 1; i < image.width() - 1; i++){
        for(int j = 1; j < image.height() - 1; j++){
            bool local_window[3][3] = {false};
            bool isWindowEmpty = true;

            for(int m = -1; m <= 1; m++){
                for(int n = -1; n <= 1; n++){
                    QRgb rgbValue = image.pixel(i + m, j + n);
                    if (rgbValue != 0xffffffff) {
                        local_window[m + 1][n + 1] = true;
                        isWindowEmpty = false;
                    }
                }
            }

            if (isWindowEmpty || !local_window[1][1]) {
                continue; // Skip empty windows or windows not centered on a pixel
            }

            CustomMatrix mat(local_window);
            int neighborCount = mat.countTrueNeighbors();

            // Classify and store the center point (i, j)
            if (neighborCount == 1) {
                // endpoint pattern
                m_endpointPixels.append(QPoint(i, j));
            } else if (neighborCount == 2) {
                bool isStraight = (mat.mat[0][0] && mat.mat[2][2]) || // Diagonal
                                  (mat.mat[0][2] && mat.mat[2][0]) || // Diagonal /
                                  (mat.mat[0][1] && mat.mat[2][1]) || // Vertical |
                                  (mat.mat[1][0] && mat.mat[1][2]);   // Horizontal -
                if (isStraight) {
                    // body/middle window pattern
                    m_bodyMiddlePixels.append(QPoint(i, j));
                } else {
                    // corner pattern
                    m_cornerPixels.append(QPoint(i, j));
                }
            } else if (neighborCount >= 3) {
                // intersection pattern
                m_intersectionPixels.append(QPoint(i, j));
            }
        }
    }
    cout << "\n--- Line Segment Classification Results ---" << endl;
    cout << "Endpoints found: \t\t" << m_endpointPixels.size() << endl;
    cout << "Body/Middle parts found: \t" << m_bodyMiddlePixels.size() << endl;
    cout << "Corners found: \t\t" << m_cornerPixels.size() << endl;
    cout << "Intersections found: \t\t" << m_intersectionPixels.size() << endl;

    m_detectionVisualizationActive = true;

    update();

    return;
}

void DrawingCanvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up the pen and brush for drawing the points
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));

    // Draw a small circle at each stored point
    for (const QPoint& point : std::as_const(m_points)) {
        painter.drawEllipse(point, 3, 3);
    }

    if(isPaintLinesClicked){
        cout << "paint lines block is called" << endl;
        pen.setColor(Qt::red);
        pen.setWidth(1); // 4-pixel wide line
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        // Set the painter's pen to our custom pen.
        painter.setPen(pen);

        for(int i=0;i<m_points.size()-1;i+=2){
            //cout << m_points[i].x() << endl;
            painter.drawLine(m_points[i], m_points[i+1]);
        }
        isPaintLinesClicked = false;

        //return painter pen to blue
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }

    if (m_detectionVisualizationActive) {
        painter.setPen(QPen(Qt::magenta, 1));
        painter.setBrush(Qt::NoBrush);

        // Draw all Endpoint candidates
        for (const QPoint& p : std::as_const(m_endpointPixels)) {
            painter.drawRect(p.x() - 2, p.y() - 2, 4, 4);
        }

        // Draw all Corner candidates
        for (const QPoint& p : std::as_const(m_cornerPixels)) {
            painter.drawRect(p.x() - 2, p.y() - 2, 4, 4);
        }

        // Draw all Body/Middle candidates
        for (const QPoint& p : std::as_const(m_bodyMiddlePixels)) {
            painter.drawRect(p.x() - 2, p.y() - 2, 4, 4);
        }

        // Draw all Intersection candidates
        for (const QPoint& p : std::as_const(m_intersectionPixels)) {
            painter.drawRect(p.x() - 2, p.y() - 2, 4, 4);
        }
    }



}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    // Add the mouse click position to our vector of points
    m_points.append(event->pos());
    // Trigger a repaint
    update();
}


