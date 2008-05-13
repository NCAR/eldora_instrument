from PyQt4.QtCore        import *
from PyQt4.QtGui         import *

from EldoraHeader        import *
from Ui_EldoraHeaderView import *

######################################################################################
class EldoraHeaderView(QDialog, Ui_EldoraHeaderView):
    '''EldoraHeaderView presnts a static view of
    an EldoraHeader. a tab widget provides two tabs. One
    tab displays the scientific 'verify' information,
    and the other provides the complete decoding of the header.
    The two views are rendered in a QTabWidget.
    '''
    def __init__(self, parent, header):
                # initialize
        super(EldoraHeaderView, self).__init__(parent)
        self.setupUi(self)
        # fill the fields tree with the header fields
        self.fillTreeWidget(self.fieldsTree, header)
        # fill the summary tree with the verify items
        self.fillTreeWidget(self.summaryTree, header.verifyBlocks)

######################################################################################
    def fillTreeWidget(self, tree, items):
        ''' Add the header information into the tree widget
        '''
        font = QFont('Courier',12)
        font.setBold(True)
        metrics = QFontMetrics(font)
        charWidth = metrics.width(' ')
        charHeight = metrics.height()
        col0size = QSize(20*charWidth, charHeight)
        col1size = QSize(60*charWidth, charHeight)
        tree.setColumnCount(2)
        labels = [QString('Item'), QString('Values')]
        tree.setHeaderLabels(labels)
        for b in items:
            bitem = QTreeWidgetItem(tree)
            bitem.setFont(0,font)
            bitem.setSizeHint(0, col0size)
            bitem.setText(0, QString(b.description))
            for name in b.keys():
                text = b[name][0] + ': '
                while len(text)+len(b[name][1]) < 60:
                    text = text + ' '
                text = text +  b[name][1]
                fitem = QTreeWidgetItem(bitem)
                fitem.setFont(1,font)
                fitem.setSizeHint(1,col1size)
                fitem.setText(1, QString(text))
                
