// furry.h
// https://twitter.com/Mauzymice

#ifndef _FURRY_H
#define _FURRY_H

#define TEST_IMAGE_SIZE 50

const char * test_image [TEST_IMAGE_SIZE] = 
{

"MMMMMMMWMMMMMMMMMMMMMMMMMMWMMMMMMMMMMMWWWNNWWMMMMMMMMMMMMMMMMMMMMMMMMWWWMMMMMMMMMMMMMMMMMMMMWWMMMMMM\r\n",
"MMMMMMKdOWMMMMMMMMMMMMMMWKdkNWMMMMMMWXxloddoodkOKNWWMMMMMMMMMMMMMMMNOoxXWMMMMMMMMMMMMMMMMMMW0okNMMMM\r\n",
"MMMMMWx;kWMMMMMMMMMMMMMMMNx:l0WMMMMMMNdcOXXKOkdolllox0NWMMMMMMMMMW0l:dXWMMMMMMMMMMMMMMMMMMMM0;lXMMMM\r\n",
"MMMMMXccKMMMMMMMMMMMMMMMMMWKo:dXWMMMMWx:OWWWMMMWNK0koccd0NMMMMMMW0:c0WMMMMMMMMMMMMMMMMMMMMMM0;;0MMMM\r\n",
"MMMMWk;xWMMMMMMMMMMMMMMMMMMMNk:cONWMMW0:lXWWMMMMMMMMWN0dclkXWWMW0::0WMMMMMMMMMMMMMMMMMMMMMMMXlcXMMMM\r\n",
"MMMMXl:KMMMMMMMMMMMMMMMMMMMMMWKo:oKWWMWx;dXWMMMMMMWWWWMWXxlcoON0cc0WMMMMMMMMMMMMMMMMMMMMMMMMNolXMMMM\r\n",
"MMMM0:dNMMMMMMMMMMMMMMMMMMMMMMMWOlcdKWWXo,oKNWMMMMMMWMMMMWN0d:c::OWMMMMMMMMMMMMMMMMMMMMMMMMMNolXMMMM\r\n",
"MMMM0cxWMMMMMMMMMMMMMMMMMMMMMMMMWNk::dxoc;,:kNMMMMMMMMMMMMMMW0c,cKMMMMMMMMMMMMMMMMMMMMMMMMMMNolXMMMM\r\n",
"MMMM0cxWMMMMMMMMMMMMMMMMMMMMMMMMWN0l,cdOKXKKNWWWMMMMMMMMMMMMMMNKKWMMMMMMMMMMMMMMMMMMMMMMMMMMNolXMMMM\r\n",
"MMMM0cxWMMMMMMMMMMMMMMMMMMMMMMWKd:,:xKNNNXKKKKKXWMMMMMMMMMMMMMMMMWMMMMMMMMMMMMMMMMMMMMMMMMMMNolXMMMM\r\n",
"MMMM0cxWMMMMMMMMMMMMMMMMMMMMMMNOollodddddodxddxONMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNocXMMMM\r\n",
"MMMM0cxWMMMMMMMMMMMMMMMMMMMMMMWWWWWNNNNNNNWWWWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMKclXMMMM\r\n",
"MMMM0cxWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM0:dWMMMM\r\n",
"MMMM0:oNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM0:dWMMMM\r\n",
"MMMMXlcXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWd;kWMMMM\r\n",
"MMMMNo:0MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWO;lXMMMMM\r\n",
"MMMMWk;xWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWWMMWWWWWWWWWWWWWMMMMMMMMMMMXc:KWMMMMM\r\n",
"MMMMMXlcKMMMMMMMMWWWWWWWNXXXXXNNNXXXNNWMMMMMMMMMMMMMMMMMMN0kxxxdooloddddlcldoo0WMMMMMMMMMNd;kWMMMMMM\r\n",
"MMMMMWO:oNMMMMMMWKxxdlodddxxo:;;,,,,;:dXMMMMMMMMMMMMMMMMWk'.......,xXXXXO:;kK0XWMMMMMMMWXd;dNMMMMMMM\r\n",
"MMMMMMNx;dNMMMMMWNX0c;xXNWWNk'........,OWMMMMMMMMMMMMMMMNl.........xWMMMW0:cKMMMMMMMWN0dcckNMMMMMMMM\r\n",
"MMMMMMMNd;dXMMMMMMWx;xWMMMMWx.........,OWMMMMMMMMMMMMMMMXc.........xWMMMMWk;dWMMMMMWO:,:d0NNNWWMMMMM\r\n",
"WXXNWWMWNk:cONMMMMKccXMMMMMWd.........,OMMMMMMMMMMMMMMMMXc.........xWMMMMMXlcXMMMXxoc,:oxxxdocxNMMMM\r\n",
"Kl,codddxxo:cxXWMW0:dWMMMMMWd.........,OWMMMMMMMMMMMMMMMXl.........xWMMMMMWdcKMMMN00KKNWWNNKo,dNMMMM\r\n",
"Wk;:k000OOOkdoOWMWx:OMMMMMMWk'........:KMMMMMMMMMMMMMMMMNo........:KMMMMMMWdcKMMMMMMMMMMMNkcckNMMMMM\r\n",
"WWO:lONMMMMMWWWWMWx:OWMMMMMMK:.......'kWMMMMMMMMMMMMMMMMMKc......;kWMMMMMMNocKMMMMMMMMWNOl:xXWMMMMMM\r\n",
"MMWKdclkXWMMMMMMMWO:oNMMMMMMW0l.....cONMMWWNNNWWMMMMMMMMMWKxllllxKWMMMMMMMXooXMMMMMMMNOl:dKWMMMMMMMM\r\n",
"MMMMWKdccd0NMMMMWWNxoKMMMMMMMMN0xddONWMMW0dlcco0WMMMMMMMMMMMMMMWMMMMMMWWNNWXNWWWMMMWXo:dKWMMMMMMMMMM\r\n",
"MMMMMMWNx,:0WWKOOkOKNWWNWWMMMMMMMMMMMMMMWNKKKKKNMMMMMMMMMMMMMMMMMMMMWNKxoOX0kOXWMMMWKlcOWMMMMMMMMMMM\r\n",
"MMMMMMMNd;kWWKkxOdlk0kxkKWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWMMMMMMMMMW0xdooddkXWWWMMMWXo:dXWMMMMMMMMM\r\n",
"MMMMMMNx;dNMWWNWXdloxOKXWWMMMMMMMMMMMMMMMMMWNKKKXNWWWMMWXxxXMMMMMMMMWWNNOxOXWWWWWMMMMMNk:l0WWWMMMMMM\r\n",
"MMMMMWk;oNMMMMMMWXKNWWMMMMMMMMMMMMMMWK0XNNKxlloooooodxxoclkNMMMMMMMMMMMWWWWWWWWWWMMMMMMW0c:xNMMMMMMM\r\n",
"MMMMMXlcKMMMMMMMMMMMMMMMMMMMMMMMMMMMWOollolokXWWNXXK0OOOKNWMMMMMMMMMMMWMMWMWXNWMMMMMWWWNXk;:0MMMMMMM\r\n",
"MMMMMXc:k0OOOkkkxoxKNWMMMMMMMMMMMMMMMWWXKXNWMMMMMMMMMMMMMMMMMMMMMMMMMMWWWKkllk0kxxkxdoddoooxXWMMMMMM\r\n",
"MMMMMW0xddddddxkxolllok0XNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWNKOxdoclxKNKkkkkkk0XNNWMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMWN0xoooccldxk0NMMMMMMMMMMMMMMMMMMMMMMMMMMMWWNOdoooooxk0XWWWMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMWKc.,;,',lkkOOO0NMMMMMMMMMMMMMMMMMMMMMMN0kkOKNWMMWWMMMMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMNk;l0XKOxxxxxxONMMMMMMMMMMMMMMMMMMMMMMMMMXdckNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMWOc:kNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKd;oXWMMMMMMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMWXxccx0NWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWk:lKMMMMMMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMWXkc,lXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWO:lXMMMMMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMWXdcckNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWk:dNMMMMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMMNOccONMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNd;xNMMMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMWk,;xkkkkKWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNo;xWMMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMW0odxdd:,xWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl:0MMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWO:lXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWO:dWMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKl:0MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXlcKMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNd;kWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWx:OMMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMKccXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWO:dWMMMMMMMMMMMMMMMMMMMMMM\r\n",
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMWk;dWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXc:KMMMMMMMMMMMMMMMMMMMMMM\r\n"

};

#endif  /*_FURRY_H*/