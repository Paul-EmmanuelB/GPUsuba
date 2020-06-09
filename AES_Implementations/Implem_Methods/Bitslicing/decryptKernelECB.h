#ifndef _DECRYPT_ECB_H_
#define _DECRYPT_ECB_H_

#include "BtsUtils.h"
#include "invsubBytes.h"
#include "invroundKeyMixColumns.h"

__global__ static void decrypt_Kernel( uint32_t* dev_input, uint32_t* dev_output, 
size_t inputSize)
{
    // Index calculations
    int tid         = threadIdx.y*blockDim.x + threadIdx.x;     //local id
    int x           = blockIdx.x * blockDim.x + threadIdx.x;    //global x id
    int y           = blockIdx.y * blockDim.y + threadIdx.y;    //global y id
    int xwidth      = blockDim.x * gridDim.x;                   //X width of the grid
    int ywidth      = blockDim.y * gridDim.y;
    int global_tid  = y*xwidth + x;                                  //global id
    int block_width = blockDim.x * blockDim.y;                  //block width
    

    // store the expended key in shared memory.
    __shared__ uint32_t sm_key[1408];

    for(int i = tid; i < 1408; i += block_width) {
        //load the extended key into the shared memory
        sm_key[i]   = const_expkey[i];
    }
    __syncthreads();

    // Each thread treat 512 bytes.
    int elements = inputSize/512;
    int gridSize = xwidth*ywidth;

    for(int i = global_tid; i < elements; i += gridSize) {
        //Loading plaintext
        int r0   = dev_input[128*global_tid+0];
        int r1   = dev_input[128*global_tid+1];
        int r2   = dev_input[128*global_tid+2];
        int r3   = dev_input[128*global_tid+3];
        int r4   = dev_input[128*global_tid+4];
        int r5   = dev_input[128*global_tid+5];
        int r6   = dev_input[128*global_tid+6];
        int r7   = dev_input[128*global_tid+7];
        int r8   = dev_input[128*global_tid+8];
        int r9   = dev_input[128*global_tid+9];
        int r10  = dev_input[128*global_tid+10];
        int r11  = dev_input[128*global_tid+11];
        int r12  = dev_input[128*global_tid+12];
        int r13  = dev_input[128*global_tid+13];
        int r14  = dev_input[128*global_tid+14];
        int r15  = dev_input[128*global_tid+15];
        int r16  = dev_input[128*global_tid+16];
        int r17  = dev_input[128*global_tid+17];
        int r18  = dev_input[128*global_tid+18];
        int r19  = dev_input[128*global_tid+19];
        int r20  = dev_input[128*global_tid+20];
        int r21  = dev_input[128*global_tid+21];
        int r22  = dev_input[128*global_tid+22];
        int r23  = dev_input[128*global_tid+23];
        int r24  = dev_input[128*global_tid+24];
        int r25  = dev_input[128*global_tid+25];
        int r26  = dev_input[128*global_tid+26];
        int r27  = dev_input[128*global_tid+27];
        int r28  = dev_input[128*global_tid+28];
        int r29  = dev_input[128*global_tid+29];
        int r30  = dev_input[128*global_tid+30];
        int r31  = dev_input[128*global_tid+31];
        int r32  = dev_input[128*global_tid+32];
        int r33  = dev_input[128*global_tid+33];
        int r34  = dev_input[128*global_tid+34];
        int r35  = dev_input[128*global_tid+35];
        int r36  = dev_input[128*global_tid+36];
        int r37  = dev_input[128*global_tid+37];
        int r38  = dev_input[128*global_tid+38];
        int r39  = dev_input[128*global_tid+39];
        int r40  = dev_input[128*global_tid+40];
        int r41  = dev_input[128*global_tid+41];
        int r42  = dev_input[128*global_tid+42];
        int r43  = dev_input[128*global_tid+43];
        int r44  = dev_input[128*global_tid+44];
        int r45  = dev_input[128*global_tid+45];
        int r46  = dev_input[128*global_tid+46];
        int r47  = dev_input[128*global_tid+47];
        int r48  = dev_input[128*global_tid+48];
        int r49  = dev_input[128*global_tid+49];
        int r50  = dev_input[128*global_tid+50];
        int r51  = dev_input[128*global_tid+51];
        int r52  = dev_input[128*global_tid+52];
        int r53  = dev_input[128*global_tid+53];
        int r54  = dev_input[128*global_tid+54];
        int r55  = dev_input[128*global_tid+55];
        int r56  = dev_input[128*global_tid+56];
        int r57  = dev_input[128*global_tid+57];
        int r58  = dev_input[128*global_tid+58];
        int r59  = dev_input[128*global_tid+59];
        int r60  = dev_input[128*global_tid+60];
        int r61  = dev_input[128*global_tid+61];
        int r62  = dev_input[128*global_tid+62];
        int r63  = dev_input[128*global_tid+63];
        int r64  = dev_input[128*global_tid+64];
        int r65  = dev_input[128*global_tid+65];
        int r66  = dev_input[128*global_tid+66];
        int r67  = dev_input[128*global_tid+67];
        int r68  = dev_input[128*global_tid+68];
        int r69  = dev_input[128*global_tid+69];
        int r70  = dev_input[128*global_tid+70];
        int r71  = dev_input[128*global_tid+71];
        int r72  = dev_input[128*global_tid+72];
        int r73  = dev_input[128*global_tid+73];
        int r74  = dev_input[128*global_tid+74];
        int r75  = dev_input[128*global_tid+75];
        int r76  = dev_input[128*global_tid+76];
        int r77  = dev_input[128*global_tid+77];
        int r78  = dev_input[128*global_tid+78];
        int r79  = dev_input[128*global_tid+79];
        int r80  = dev_input[128*global_tid+80];
        int r81  = dev_input[128*global_tid+81];
        int r82  = dev_input[128*global_tid+82];
        int r83  = dev_input[128*global_tid+83];
        int r84  = dev_input[128*global_tid+84];
        int r85  = dev_input[128*global_tid+85];
        int r86  = dev_input[128*global_tid+86];
        int r87  = dev_input[128*global_tid+87];
        int r88  = dev_input[128*global_tid+88];
        int r89  = dev_input[128*global_tid+89];
        int r90  = dev_input[128*global_tid+90];
        int r91  = dev_input[128*global_tid+91];
        int r92  = dev_input[128*global_tid+92];
        int r93  = dev_input[128*global_tid+93];
        int r94  = dev_input[128*global_tid+94];
        int r95  = dev_input[128*global_tid+95];
        int r96  = dev_input[128*global_tid+96];
        int r97  = dev_input[128*global_tid+97];
        int r98  = dev_input[128*global_tid+98];
        int r99  = dev_input[128*global_tid+99];
        int r100 = dev_input[128*global_tid+100];
        int r101 = dev_input[128*global_tid+101];
        int r102 = dev_input[128*global_tid+102];
        int r103 = dev_input[128*global_tid+103];
        int r104 = dev_input[128*global_tid+104];
        int r105 = dev_input[128*global_tid+105];
        int r106 = dev_input[128*global_tid+106];
        int r107 = dev_input[128*global_tid+107];
        int r108 = dev_input[128*global_tid+108];
        int r109 = dev_input[128*global_tid+109];
        int r110 = dev_input[128*global_tid+110];
        int r111 = dev_input[128*global_tid+111];
        int r112 = dev_input[128*global_tid+112];
        int r113 = dev_input[128*global_tid+113];
        int r114 = dev_input[128*global_tid+114];
        int r115 = dev_input[128*global_tid+115];
        int r116 = dev_input[128*global_tid+116];
        int r117 = dev_input[128*global_tid+117];
        int r118 = dev_input[128*global_tid+118];
        int r119 = dev_input[128*global_tid+119];
        int r120 = dev_input[128*global_tid+120];
        int r121 = dev_input[128*global_tid+121];
        int r122 = dev_input[128*global_tid+122];
        int r123 = dev_input[128*global_tid+123];
        int r124 = dev_input[128*global_tid+124];
        int r125 = dev_input[128*global_tid+125];
        int r126 = dev_input[128*global_tid+126];
        int r127 = dev_input[128*global_tid+127];

        // First round AddRoundKey: ex-or with round key
        r0   ^= sm_key[1280  ];
        r1   ^= sm_key[1281  ];
        r2   ^= sm_key[1282  ];
        r3   ^= sm_key[1283  ];
        r4   ^= sm_key[1284  ];
        r5   ^= sm_key[1285  ];
        r6   ^= sm_key[1286  ];
        r7   ^= sm_key[1287  ];
        r8   ^= sm_key[1288  ];
        r9   ^= sm_key[1289  ];
        r10  ^= sm_key[1290 ];
        r11  ^= sm_key[1291 ];
        r12  ^= sm_key[1292 ];
        r13  ^= sm_key[1293 ];
        r14  ^= sm_key[1294 ];
        r15  ^= sm_key[1295 ];
        r16  ^= sm_key[1296 ];
        r17  ^= sm_key[1297 ];
        r18  ^= sm_key[1298 ];
        r19  ^= sm_key[1299 ];
        r20  ^= sm_key[1300 ];
        r21  ^= sm_key[1301 ];
        r22  ^= sm_key[1302 ];
        r23  ^= sm_key[1303 ];
        r24  ^= sm_key[1304 ];
        r25  ^= sm_key[1305 ];
        r26  ^= sm_key[1306 ];
        r27  ^= sm_key[1307 ];
        r28  ^= sm_key[1308 ];
        r29  ^= sm_key[1309 ];
        r30  ^= sm_key[1310 ];
        r31  ^= sm_key[1311 ];
        r32  ^= sm_key[1312 ];
        r33  ^= sm_key[1313 ];
        r34  ^= sm_key[1314 ];
        r35  ^= sm_key[1315 ];
        r36  ^= sm_key[1316 ];
        r37  ^= sm_key[1317 ];
        r38  ^= sm_key[1318 ];
        r39  ^= sm_key[1319 ];
        r40  ^= sm_key[1320 ];
        r41  ^= sm_key[1321 ];
        r42  ^= sm_key[1322 ];
        r43  ^= sm_key[1323 ];
        r44  ^= sm_key[1324 ];
        r45  ^= sm_key[1325 ];
        r46  ^= sm_key[1326 ];
        r47  ^= sm_key[1327 ];
        r48  ^= sm_key[1328 ];
        r49  ^= sm_key[1329 ];
        r50  ^= sm_key[1330 ];
        r51  ^= sm_key[1331 ];
        r52  ^= sm_key[1332 ];
        r53  ^= sm_key[1333 ];
        r54  ^= sm_key[1334 ];
        r55  ^= sm_key[1335 ];
        r56  ^= sm_key[1336 ];
        r57  ^= sm_key[1337 ];
        r58  ^= sm_key[1338 ];
        r59  ^= sm_key[1339 ];
        r60  ^= sm_key[1340 ];
        r61  ^= sm_key[1341 ];
        r62  ^= sm_key[1342 ];
        r63  ^= sm_key[1343 ];
        r64  ^= sm_key[1344 ];
        r65  ^= sm_key[1345 ];
        r66  ^= sm_key[1346 ];
        r67  ^= sm_key[1347 ];
        r68  ^= sm_key[1348 ];
        r69  ^= sm_key[1349 ];
        r70  ^= sm_key[1350 ];
        r71  ^= sm_key[1351 ];
        r72  ^= sm_key[1352 ];
        r73  ^= sm_key[1353 ];
        r74  ^= sm_key[1354 ];
        r75  ^= sm_key[1355 ];
        r76  ^= sm_key[1356 ];
        r77  ^= sm_key[1357 ];
        r78  ^= sm_key[1358 ];
        r79  ^= sm_key[1359 ];
        r80  ^= sm_key[1360 ];
        r81  ^= sm_key[1361 ];
        r82  ^= sm_key[1362 ];
        r83  ^= sm_key[1363 ];
        r84  ^= sm_key[1364 ];
        r85  ^= sm_key[1365 ];
        r86  ^= sm_key[1366 ];
        r87  ^= sm_key[1367 ];
        r88  ^= sm_key[1368 ];
        r89  ^= sm_key[1369 ];
        r90  ^= sm_key[1370 ];
        r91  ^= sm_key[1371 ];
        r92  ^= sm_key[1372 ];
        r93  ^= sm_key[1373 ];
        r94  ^= sm_key[1374 ];
        r95  ^= sm_key[1375 ];
        r96  ^= sm_key[1376 ];
        r97  ^= sm_key[1377 ];
        r98  ^= sm_key[1378 ];
        r99  ^= sm_key[1379 ];
        r100 ^= sm_key[1380];
        r101 ^= sm_key[1381];
        r102 ^= sm_key[1382];
        r103 ^= sm_key[1383];
        r104 ^= sm_key[1384];
        r105 ^= sm_key[1385];
        r106 ^= sm_key[1386];
        r107 ^= sm_key[1387];
        r108 ^= sm_key[1388];
        r109 ^= sm_key[1389];
        r110 ^= sm_key[1390];
        r111 ^= sm_key[1391];
        r112 ^= sm_key[1392];
        r113 ^= sm_key[1393];
        r114 ^= sm_key[1394];
        r115 ^= sm_key[1395];
        r116 ^= sm_key[1396];
        r117 ^= sm_key[1397];
        r118 ^= sm_key[1398];
        r119 ^= sm_key[1399];
        r120 ^= sm_key[1400];
        r121 ^= sm_key[1401];
        r122 ^= sm_key[1402];
        r123 ^= sm_key[1403];
        r124 ^= sm_key[1404];
        r125 ^= sm_key[1405];
        r126 ^= sm_key[1406];
        r127 ^= sm_key[1407];

        // Round transformation: a set of table lookups operations.
        #pragma unroll
        for (int i = 9; i; i--) {
            //subBytes
            inverseSbox( r0  , r1  , r2  , r3  , r4  , r5  , r6  , r7  );
            inverseSbox( r8  , r9  , r10 , r11 , r12 , r13 , r14 , r15 );
            inverseSbox( r16 , r17 , r18 , r19 , r20 , r21 , r22 , r23 );
            inverseSbox( r24 , r25 , r26 , r27 , r28 , r29 , r30 , r31 );
            inverseSbox( r32 , r33 , r34 , r35 , r36 , r37 , r38 , r39 );
            inverseSbox( r40 , r41 , r42 , r43 , r44 , r45 , r46 , r47 );
            inverseSbox( r48 , r49 , r50 , r51 , r52 , r53 , r54 , r55 );
            inverseSbox( r56 , r57 , r58 , r59 , r60 , r61 , r62 , r63 );
            inverseSbox( r64 , r65 , r66 , r67 , r68 , r69 , r70 , r71 );
            inverseSbox( r72 , r73 , r74 , r75 , r76 , r77 , r78 , r79 );
            inverseSbox( r80 , r81 , r82 , r83 , r84 , r85 , r86 , r87 );
            inverseSbox( r88 , r89 , r90 , r91 , r92 , r93 , r94 , r95 );
            inverseSbox( r96 , r97 , r98 , r99 , r100, r101, r102, r103);
            inverseSbox( r104, r105, r106, r107, r108, r109, r110, r111);
            inverseSbox( r112, r113, r114, r115, r116, r117, r118, r119);
            inverseSbox( r120, r121, r122, r123, r124, r125, r126, r127);
    
            //The shiftrow step correspond to the parameters order
            InvKeyMixcl(r0  , r1  , r2  , r3  , r4  , r5  , r6  , r7  ,
                        r56 , r57 , r58 , r59 , r60 , r61 , r62 , r63 ,
                        r80 , r81 , r82 , r83 , r84 , r85 , r86 , r87 ,
                        r104, r105, r106, r107, r108, r109, r110, r111,
                     sm_key[i*128+0  ], sm_key[i*128+1  ], sm_key[i*128+2  ], sm_key[i*128+3  ],
                     sm_key[i*128+4  ], sm_key[i*128+5  ], sm_key[i*128+6  ], sm_key[i*128+7  ],
                     sm_key[i*128+32 ], sm_key[i*128+33 ], sm_key[i*128+34 ], sm_key[i*128+35 ],
                     sm_key[i*128+36 ], sm_key[i*128+37 ], sm_key[i*128+38 ], sm_key[i*128+39 ],
                     sm_key[i*128+64 ], sm_key[i*128+65 ], sm_key[i*128+66 ], sm_key[i*128+67 ],
                     sm_key[i*128+68 ], sm_key[i*128+69 ], sm_key[i*128+70 ], sm_key[i*128+71 ],
                     sm_key[i*128+96 ], sm_key[i*128+97 ], sm_key[i*128+98 ], sm_key[i*128+99 ],
                     sm_key[i*128+100], sm_key[i*128+101], sm_key[i*128+102], sm_key[i*128+103]);
            InvKeyMixcl(r8  , r9  , r10 , r11 , r12 , r13 , r14 , r15 ,
                        r32 , r33 , r34 , r35 , r36 , r37 , r38 , r39 ,
                        r88 , r89 , r90 , r91 , r92 , r93 , r94 , r95 ,
                        r112, r113, r114, r115, r116, r117, r118, r119,
                     sm_key[i*128+8  ], sm_key[i*128+9  ], sm_key[i*128+10 ], sm_key[i*128+11 ],
                     sm_key[i*128+12 ], sm_key[i*128+13 ], sm_key[i*128+14 ], sm_key[i*128+15 ],
                     sm_key[i*128+40 ], sm_key[i*128+41 ], sm_key[i*128+42 ], sm_key[i*128+43 ],
                     sm_key[i*128+44 ], sm_key[i*128+45 ], sm_key[i*128+46 ], sm_key[i*128+47 ],
                     sm_key[i*128+72 ], sm_key[i*128+73 ], sm_key[i*128+74 ], sm_key[i*128+75 ],
                     sm_key[i*128+76 ], sm_key[i*128+77 ], sm_key[i*128+78 ], sm_key[i*128+79 ],
                     sm_key[i*128+104], sm_key[i*128+105], sm_key[i*128+106], sm_key[i*128+107],
                     sm_key[i*128+108], sm_key[i*128+109], sm_key[i*128+110], sm_key[i*128+111]);
            InvKeyMixcl(r16 , r17 , r18 , r19 , r20 , r21 , r22 , r23 ,
                        r40 , r41 , r42 , r43 , r44 , r45 , r46 , r47 ,
                        r64 , r65 , r66 , r67 , r68 , r69 , r70 , r71 ,
                        r120, r121, r122, r123, r124, r125, r126, r127,
                     sm_key[i*128+16 ], sm_key[i*128+17 ], sm_key[i*128+18 ], sm_key[i*128+19 ],
                     sm_key[i*128+20 ], sm_key[i*128+21 ], sm_key[i*128+22 ], sm_key[i*128+23 ],
                     sm_key[i*128+48 ], sm_key[i*128+49 ], sm_key[i*128+50 ], sm_key[i*128+51 ],
                     sm_key[i*128+52 ], sm_key[i*128+53 ], sm_key[i*128+54 ], sm_key[i*128+55 ],
                     sm_key[i*128+80 ], sm_key[i*128+81 ], sm_key[i*128+82 ], sm_key[i*128+83 ],
                     sm_key[i*128+84 ], sm_key[i*128+85 ], sm_key[i*128+86 ], sm_key[i*128+87 ],
                     sm_key[i*128+112], sm_key[i*128+113], sm_key[i*128+114], sm_key[i*128+115],
                     sm_key[i*128+116], sm_key[i*128+117], sm_key[i*128+118], sm_key[i*128+119]);
            InvKeyMixcl(r24 , r25 , r26 , r27 , r28 , r29 , r30 , r31 ,
                        r48 , r49 , r50 , r51 , r52 , r53 , r54 , r55 ,
                        r72 , r73 , r74 , r75 , r76 , r77 , r78 , r79 ,
                        r96 , r97 , r98 , r99 , r100, r101, r102, r103,
                     sm_key[i*128+24 ], sm_key[i*128+25 ], sm_key[i*128+26 ], sm_key[i*128+27 ],
                     sm_key[i*128+28 ], sm_key[i*128+29 ], sm_key[i*128+30 ], sm_key[i*128+31 ],
                     sm_key[i*128+56 ], sm_key[i*128+57 ], sm_key[i*128+58 ], sm_key[i*128+59 ],
                     sm_key[i*128+60 ], sm_key[i*128+61 ], sm_key[i*128+62 ], sm_key[i*128+63 ],
                     sm_key[i*128+88 ], sm_key[i*128+89 ], sm_key[i*128+90 ], sm_key[i*128+91 ],
                     sm_key[i*128+92 ], sm_key[i*128+93 ], sm_key[i*128+94 ], sm_key[i*128+95 ],
                     sm_key[i*128+120], sm_key[i*128+121], sm_key[i*128+122], sm_key[i*128+123],
                     sm_key[i*128+124], sm_key[i*128+125], sm_key[i*128+126], sm_key[i*128+127]);
        } 

        // The final round doesn’t include the MixColumns step
        //subBytes
        inverseSbox( r0  , r1  , r2  , r3  , r4  , r5  , r6  , r7  );
        inverseSbox( r8  , r9  , r10 , r11 , r12 , r13 , r14 , r15 );
        inverseSbox( r16 , r17 , r18 , r19 , r20 , r21 , r22 , r23 );
        inverseSbox( r24 , r25 , r26 , r27 , r28 , r29 , r30 , r31 );
        inverseSbox( r32 , r33 , r34 , r35 , r36 , r37 , r38 , r39 );
        inverseSbox( r40 , r41 , r42 , r43 , r44 , r45 , r46 , r47 );
        inverseSbox( r48 , r49 , r50 , r51 , r52 , r53 , r54 , r55 );
        inverseSbox( r56 , r57 , r58 , r59 , r60 , r61 , r62 , r63 );
        inverseSbox( r64 , r65 , r66 , r67 , r68 , r69 , r70 , r71 );
        inverseSbox( r72 , r73 , r74 , r75 , r76 , r77 , r78 , r79 );
        inverseSbox( r80 , r81 , r82 , r83 , r84 , r85 , r86 , r87 );
        inverseSbox( r88 , r89 , r90 , r91 , r92 , r93 , r94 , r95 );
        inverseSbox( r96 , r97 , r98 , r99 , r100, r101, r102, r103);
        inverseSbox( r104, r105, r106, r107, r108, r109, r110, r111);
        inverseSbox( r112, r113, r114, r115, r116, r117, r118, r119);
        inverseSbox( r120, r121, r122, r123, r124, r125, r126, r127);

        
        //ShiftRows & AddRoundKey: ex-or with round key
        int temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;
        // First Row
        r0   ^= sm_key[0 ];
        r1   ^= sm_key[1 ];
        r2   ^= sm_key[2 ];
        r3   ^= sm_key[3 ];
        r4   ^= sm_key[4 ];
        r5   ^= sm_key[5 ];
        r6   ^= sm_key[6 ];
        r7   ^= sm_key[7 ];
        r8   ^= sm_key[8 ];
        r9   ^= sm_key[9 ];
        r10  ^= sm_key[10];
        r11  ^= sm_key[11];
        r12  ^= sm_key[12];
        r13  ^= sm_key[13];
        r14  ^= sm_key[14];
        r15  ^= sm_key[15];
        r16  ^= sm_key[16];
        r17  ^= sm_key[17];
        r18  ^= sm_key[18];
        r19  ^= sm_key[19];
        r20  ^= sm_key[20];
        r21  ^= sm_key[21];
        r22  ^= sm_key[22];
        r23  ^= sm_key[23];
        r24  ^= sm_key[24];
        r25  ^= sm_key[25];
        r26  ^= sm_key[26];
        r27  ^= sm_key[27];
        r28  ^= sm_key[28];
        r29  ^= sm_key[29];
        r30  ^= sm_key[30];
        r31  ^= sm_key[31];
        // Second Row
        temp0 = r32^sm_key[40];
        temp1 = r33^sm_key[41];
        temp2 = r34^sm_key[42];
        temp3 = r35^sm_key[43];
        temp4 = r36^sm_key[44];
        temp5 = r37^sm_key[45];
        temp6 = r38^sm_key[46];
        temp7 = r39^sm_key[47];
        r32   = r56^sm_key[32];
        r33   = r57^sm_key[33];
        r34   = r58^sm_key[34];
        r35   = r59^sm_key[35];
        r36   = r60^sm_key[36];
        r37   = r61^sm_key[37];
        r38   = r62^sm_key[38];
        r39   = r63^sm_key[39];
        r56   = r48^sm_key[56];
        r57   = r49^sm_key[57];
        r58   = r50^sm_key[58];
        r59   = r51^sm_key[59];
        r60   = r52^sm_key[60];
        r61   = r53^sm_key[61];
        r62   = r54^sm_key[62];
        r63   = r55^sm_key[63];
        r48   = r40^sm_key[48];
        r49   = r41^sm_key[49];
        r50   = r42^sm_key[50];
        r51   = r43^sm_key[51];
        r52   = r44^sm_key[52];
        r53   = r45^sm_key[53];
        r54   = r46^sm_key[54];
        r55   = r47^sm_key[55];
        r40   = temp0;
        r41   = temp1;
        r42   = temp2;
        r43   = temp3;
        r44   = temp4;
        r45   = temp5;
        r46   = temp6;
        r47   = temp7;
        //Third row
        temp0 = r64^sm_key[80];
        temp1 = r65^sm_key[81];
        temp2 = r66^sm_key[82];
        temp3 = r67^sm_key[83];
        temp4 = r68^sm_key[84];
        temp5 = r69^sm_key[85];
        temp6 = r70^sm_key[86];
        temp7 = r71^sm_key[87];
        r64   = r80^sm_key[64];
        r65   = r81^sm_key[65];
        r66   = r82^sm_key[66];
        r67   = r83^sm_key[67];
        r68   = r84^sm_key[68];
        r69   = r85^sm_key[69];
        r70   = r86^sm_key[70];
        r71   = r87^sm_key[71];
        r80   = temp0;
        r81   = temp1;
        r82   = temp2;
        r83   = temp3;
        r84   = temp4;
        r85   = temp5;
        r86   = temp6;
        r87   = temp7;
        temp0 = r72^sm_key[88];
        temp1 = r73^sm_key[89];
        temp2 = r74^sm_key[90];
        temp3 = r75^sm_key[91];
        temp4 = r76^sm_key[92];
        temp5 = r77^sm_key[93];
        temp6 = r78^sm_key[94];
        temp7 = r79^sm_key[95];
        r72   = r88^sm_key[72];
        r73   = r89^sm_key[73];
        r74   = r90^sm_key[74];
        r75   = r91^sm_key[75];
        r76   = r92^sm_key[76];
        r77   = r93^sm_key[77];
        r78   = r94^sm_key[78];
        r79   = r95^sm_key[79];
        r88   = temp0;
        r89   = temp1;
        r90   = temp2;
        r91   = temp3;
        r92   = temp4;
        r93   = temp5;
        r94   = temp6;
        r95   = temp7;
        //Last row
        temp0 = r96 ^sm_key[120];
        temp1 = r97 ^sm_key[121];
        temp2 = r98 ^sm_key[122];
        temp3 = r99 ^sm_key[123];
        temp4 = r100^sm_key[124];
        temp5 = r101^sm_key[125];
        temp6 = r102^sm_key[126];
        temp7 = r103^sm_key[127];
        r96   = r104^sm_key[96 ];
        r97   = r105^sm_key[97 ];
        r98   = r106^sm_key[98 ];
        r99   = r107^sm_key[99 ];
        r100  = r108^sm_key[100];
        r101  = r109^sm_key[101];
        r102  = r110^sm_key[102];
        r103  = r111^sm_key[103];
        r104  = r112^sm_key[104];
        r105  = r113^sm_key[105];
        r106  = r114^sm_key[106];
        r107  = r115^sm_key[107];
        r108  = r116^sm_key[108];
        r109  = r117^sm_key[109];
        r110  = r118^sm_key[110];
        r111  = r119^sm_key[111];
        r112  = r120^sm_key[112];
        r113  = r121^sm_key[113];
        r114  = r122^sm_key[114];
        r115  = r123^sm_key[115];
        r116  = r124^sm_key[116];
        r117  = r125^sm_key[117];
        r118  = r126^sm_key[118];
        r119  = r127^sm_key[119];
        r120  = temp0;
        r121  = temp1;
        r122  = temp2;
        r123  = temp3;
        r124  = temp4;
        r125  = temp5;
        r126  = temp6;
        r127  = temp7;

        //output
        dev_output[128*global_tid+0]   = r0;
        dev_output[128*global_tid+1]   = r1;
        dev_output[128*global_tid+2]   = r2;
        dev_output[128*global_tid+3]   = r3;
        dev_output[128*global_tid+4]   = r4;
        dev_output[128*global_tid+5]   = r5;
        dev_output[128*global_tid+6]   = r6;
        dev_output[128*global_tid+7]   = r7;
        dev_output[128*global_tid+8]   = r8;
        dev_output[128*global_tid+9]   = r9;    
        dev_output[128*global_tid+10]  = r10; 
        dev_output[128*global_tid+11]  = r11; 
        dev_output[128*global_tid+12]  = r12; 
        dev_output[128*global_tid+13]  = r13; 
        dev_output[128*global_tid+14]  = r14; 
        dev_output[128*global_tid+15]  = r15; 
        dev_output[128*global_tid+16]  = r16; 
        dev_output[128*global_tid+17]  = r17; 
        dev_output[128*global_tid+18]  = r18; 
        dev_output[128*global_tid+19]  = r19; 
        dev_output[128*global_tid+20]  = r20; 
        dev_output[128*global_tid+21]  = r21; 
        dev_output[128*global_tid+22]  = r22; 
        dev_output[128*global_tid+23]  = r23; 
        dev_output[128*global_tid+24]  = r24; 
        dev_output[128*global_tid+25]  = r25; 
        dev_output[128*global_tid+26]  = r26; 
        dev_output[128*global_tid+27]  = r27; 
        dev_output[128*global_tid+28]  = r28; 
        dev_output[128*global_tid+29]  = r29; 
        dev_output[128*global_tid+30]  = r30; 
        dev_output[128*global_tid+31]  = r31; 
        dev_output[128*global_tid+32]  = r32; 
        dev_output[128*global_tid+33]  = r33; 
        dev_output[128*global_tid+34]  = r34; 
        dev_output[128*global_tid+35]  = r35; 
        dev_output[128*global_tid+36]  = r36; 
        dev_output[128*global_tid+37]  = r37; 
        dev_output[128*global_tid+38]  = r38; 
        dev_output[128*global_tid+39]  = r39; 
        dev_output[128*global_tid+40]  = r40; 
        dev_output[128*global_tid+41]  = r41; 
        dev_output[128*global_tid+42]  = r42; 
        dev_output[128*global_tid+43]  = r43; 
        dev_output[128*global_tid+44]  = r44; 
        dev_output[128*global_tid+45]  = r45; 
        dev_output[128*global_tid+46]  = r46; 
        dev_output[128*global_tid+47]  = r47; 
        dev_output[128*global_tid+48]  = r48; 
        dev_output[128*global_tid+49]  = r49; 
        dev_output[128*global_tid+50]  = r50; 
        dev_output[128*global_tid+51]  = r51; 
        dev_output[128*global_tid+52]  = r52; 
        dev_output[128*global_tid+53]  = r53; 
        dev_output[128*global_tid+54]  = r54; 
        dev_output[128*global_tid+55]  = r55; 
        dev_output[128*global_tid+56]  = r56; 
        dev_output[128*global_tid+57]  = r57; 
        dev_output[128*global_tid+58]  = r58; 
        dev_output[128*global_tid+59]  = r59; 
        dev_output[128*global_tid+60]  = r60; 
        dev_output[128*global_tid+61]  = r61; 
        dev_output[128*global_tid+62]  = r62; 
        dev_output[128*global_tid+63]  = r63; 
        dev_output[128*global_tid+64]  = r64; 
        dev_output[128*global_tid+65]  = r65; 
        dev_output[128*global_tid+66]  = r66; 
        dev_output[128*global_tid+67]  = r67; 
        dev_output[128*global_tid+68]  = r68; 
        dev_output[128*global_tid+69]  = r69; 
        dev_output[128*global_tid+70]  = r70; 
        dev_output[128*global_tid+71]  = r71; 
        dev_output[128*global_tid+72]  = r72; 
        dev_output[128*global_tid+73]  = r73; 
        dev_output[128*global_tid+74]  = r74; 
        dev_output[128*global_tid+75]  = r75; 
        dev_output[128*global_tid+76]  = r76; 
        dev_output[128*global_tid+77]  = r77; 
        dev_output[128*global_tid+78]  = r78; 
        dev_output[128*global_tid+79]  = r79; 
        dev_output[128*global_tid+80]  = r80; 
        dev_output[128*global_tid+81]  = r81; 
        dev_output[128*global_tid+82]  = r82; 
        dev_output[128*global_tid+83]  = r83; 
        dev_output[128*global_tid+84]  = r84; 
        dev_output[128*global_tid+85]  = r85; 
        dev_output[128*global_tid+86]  = r86; 
        dev_output[128*global_tid+87]  = r87; 
        dev_output[128*global_tid+88]  = r88; 
        dev_output[128*global_tid+89]  = r89; 
        dev_output[128*global_tid+90]  = r90; 
        dev_output[128*global_tid+91]  = r91; 
        dev_output[128*global_tid+92]  = r92; 
        dev_output[128*global_tid+93]  = r93; 
        dev_output[128*global_tid+94]  = r94; 
        dev_output[128*global_tid+95]  = r95; 
        dev_output[128*global_tid+96]  = r96; 
        dev_output[128*global_tid+97]  = r97; 
        dev_output[128*global_tid+98]  = r98; 
        dev_output[128*global_tid+99]  = r99;
        dev_output[128*global_tid+100] = r100 ;
        dev_output[128*global_tid+101] = r101 ;
        dev_output[128*global_tid+102] = r102 ;
        dev_output[128*global_tid+103] = r103 ;
        dev_output[128*global_tid+104] = r104 ;
        dev_output[128*global_tid+105] = r105 ;
        dev_output[128*global_tid+106] = r106 ;
        dev_output[128*global_tid+107] = r107 ;
        dev_output[128*global_tid+108] = r108 ;
        dev_output[128*global_tid+109] = r109 ;
        dev_output[128*global_tid+110] = r110 ;
        dev_output[128*global_tid+111] = r111 ;
        dev_output[128*global_tid+112] = r112 ;
        dev_output[128*global_tid+113] = r113 ;
        dev_output[128*global_tid+114] = r114 ;
        dev_output[128*global_tid+115] = r115 ;
        dev_output[128*global_tid+116] = r116 ;
        dev_output[128*global_tid+117] = r117 ;
        dev_output[128*global_tid+118] = r118 ;
        dev_output[128*global_tid+119] = r119 ;
        dev_output[128*global_tid+120] = r120 ;
        dev_output[128*global_tid+121] = r121 ;
        dev_output[128*global_tid+122] = r122 ;
        dev_output[128*global_tid+123] = r123 ;
        dev_output[128*global_tid+124] = r124 ;
        dev_output[128*global_tid+125] = r125 ;
        dev_output[128*global_tid+126] = r126 ;
        dev_output[128*global_tid+127] = r127 ;
    }

}

#endif //_DECRYPT_ECB_H_