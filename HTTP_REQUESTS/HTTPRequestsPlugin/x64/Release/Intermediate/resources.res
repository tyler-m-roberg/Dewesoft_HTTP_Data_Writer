        ��  ��                  F  (   ��
 I C O N         0          �PNG

   IHDR           szz�   bKGD � � �����   	pHYs     ��   tIME��-��  �IDATX��i�E�Uӳ��M��5���׊��JE�����E{�(��A!�/L��"�E#��5�H@A��h@D�e��uٞ���5�vv�̊��b�{U���]=p��=ſF�Sv?�SC�	<�4p��y(vb��r��� �����m�Dvo�z`2�6���\?��������Ӟ:��E�v��&j_�
��7 ���^{�]��^�����XwiOmY,����,��[iTN����&)Ŝ\,�(5tI�=ޫ=5���������*���tH��������+�kOwl���N��,��e@�:�~�P8�/����О����H�X�_�jeB�X���xN{�w9�:�;�Ȣ
:N�B0�L�����8s�/���� ��%!F]?n����������R)`�����bG��'¸h�WRf�I=d�Ҟ����S�5�T �1���0�b(��&�\��fO�G�r���?���h�-@�^��Q�1�M�b���5ى��t��9�N�8���R�3�%�[�������01ca�9�Jj��A�{������n�tK��#�}]����k�y�@1S�#OO�IF��XU��:eyߒ��=� �z�rx����f=�����J��(��y��yn�X!'Y���Z���M5�����ތҫ���,X�c�3�[
��S~��L��X��U��x�}	�������6�Ȇ���~�
�oX�����fe[��S�V>�΄���+��e�Ϸ�ۻ� ^��1�6V��p?������\�l��L�Bc�4�v� ֞Z��n�Sہ!�^��L�����t�B%e�I�� X �r�`�}�,`w6$�"�x������9��\ns���$Ԟ�x��T]q�
�UdF��{���~�x'۩�X�z,O�1�v�Z���#���]3V��w�h�o�OLk��Bӳ2    IEND�B`�  �  ,   T Y P E L I B   ��     0          MSFT      	      C             ����                  ����       �   ����       H  d   ����   �     ����   �     ����   �     ����   �  �   ����   ,  �   ����        ����     8   ����   ����    ����   ����    ����   ����    ����   D  T   ����   �  $   ����   ����    ����   ����    ����   %B  �                                     `      $       ����        ����                  �������������   ��������������������������������`   ������������������������������������������������H      ��������x   ��������0   /)�C�lW�>��������e�w�|Q���  �w<�����    c�w�|Q���  �w<���������d�w�|Q���  �w<�����������:�c�L�\��vfj�    ����,�ٖ�M��f�QV�   �����)h��z!I�0n���   ����      ��������      �   x       v 3 Dewesoft.exeWW����������������������������������������������������������������$   ������������������������������������������������������������������������������������������������������������������������������������    ���������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������� �kHTTPRequestsPluginLibWWW    ����8��PluginWW >   Created by MIDL version 8.01.0622 at Mon Jan 18 20:14:07 2038
 ���WW nWW       ����0   D       H   L      �   H   M U I   U I / M O D A L _ W I N D O W . X M L       0          <?xml version="1.0" encoding="utf-8" ?>
<Grid Height="100" Width="200">
    <Button Name="closeButton">Close</Button>    
</Grid>
  �  H   M U I   U I / S E T U P _ W I N D O W . X M L       0          <?xml version="1.0" encoding="utf-8"?>
<Window xmlns="https://mui.dewesoft.com/schema/1.1">
    <Grid>
        <Grid.ColumnDefinitions>
            <ColumnDefinition Width="100%"/>
        </Grid.ColumnDefinitions>
        <Grid.RowDefinitions>
            <RowDefinition Height="290"/>
            <RowDefinition Height="100"/>
            <RowDefinition Height="100%"/>
        </Grid.RowDefinitions>

        <CaptionPanel Grid.Row="0" Grid.Column="0" Title="General Settings" Name="generalSettingsPanel">
            <Grid>
                <Grid.RowDefinitions>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                </Grid.RowDefinitions>

                <Grid.ColumnDefinitions>
                    <ColumnDefinition Width="20"/>
                    <ColumnDefinition Width="250"/>
                    <ColumnDefinition Width="10"/>
                    <ColumnDefinition Width="120"/>
                    <ColumnDefinition Width="10"/>
                    <ColumnDefinition Width="120"/>
                    <ColumnDefinition Width="40"/>
                    <ColumnDefinition Width="440"/>
                    <ColumnDefinition Width="10"/>
                </Grid.ColumnDefinitions>

                <Label Grid.Row="1" Grid.Column="1"  Text="Trigger Channel" Name="triggerChanLabel" Align="Fill" TextAlign="Center" FontWeight="Bold"/>
                <ComboBox Grid.Row="2" Grid.Column="1"  Name="triggerChanCBox" />

                <Label Grid.Row="1" Grid.Column="3"  Text="Trigger Level" Name="triggerLevelLabel" Align="Fill" TextAlign="Center" FontWeight="Bold"/>
                <TextBox Grid.Row="2" Grid.Column="3" Placeholder="" Name="triggerLevelTextBox" ValidateType="Double" />

                <Label Grid.Row="1" Grid.Column="5" Text="Edge Type" Name ="edgeTypeLabel" Align="Fill" TextAlign="Center" FontWeight="Bold"/>
                <ComboBox Grid.Row="2" Grid.Column="5" Name="edgeTypeCBox"/>

                <Label Grid.Row="4" Grid.Column="1"  Text="Template File (.xlsx)" Name="templateFileLabel" Align="Fill" TextAlign="Center" Grid.ColumnSpan="5" FontWeight="Bold"/>
                <TextBox Grid.Row="5" Grid.Column="1" Placeholder="" Name="templateFileTextBox" Grid.ColumnSpan="5"/>

                <Label Grid.Row="7" Grid.Column="1"  Text="Report Directory (Absolute or Relative)" Name="reportDirLabel" Align="Fill" TextAlign="Center" Grid.ColumnSpan="5" FontWeight="Bold"/>
                <TextBox Grid.Row="8" Grid.Column="1" Placeholder="" Name="reportDirTextBox" Grid.ColumnSpan="5"/>

                <Label Grid.Row="10" Grid.Column="1"  Text="Report Name" Name="reportNameLabel" Align="Fill" TextAlign="Center" Grid.ColumnSpan="5" FontWeight="Bold"/>
                <TextBox Grid.Row="11" Grid.Column="1" Placeholder="" Name="reportNameTextBox" Grid.ColumnSpan="5"/>

                <CaptionPanel Grid.Row="1" Grid.Column="7" Name="optionsListPanel" Grid.RowSpan="11" Title="Options List">
                    <StackPanel Name="optionsListStackPanel"/>
                </CaptionPanel>



            </Grid>
        </CaptionPanel>

        <CaptionPanel Grid.Row="1" Grid.Column="0" Title="Add Channel" Name="addChannelPanel">
            <Grid>
                <Grid.RowDefinitions>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                    <RowDefinition Height="20"/>
                </Grid.RowDefinitions>
                <Grid.ColumnDefinitions>
                    <ColumnDefinition Width="20"/>
                    <ColumnDefinition Width="200"/>
                    <ColumnDefinition Width="10"/>
                    <ColumnDefinition Width="200"/>
                    <ColumnDefinition Width="10"/>
                    <ColumnDefinition Width="360"/>
                    <ColumnDefinition Width="10"/>
                    <ColumnDefinition Width="100"/>
                    <ColumnDefinition Width="10"/>
                    <ColumnDefinition Width="100"/>
                    <ColumnDefinition Width="10"/>
                    <ColumnDefinition Width="100"/>
                </Grid.ColumnDefinitions>

                <Label Grid.Row="1" Grid.Column="1"  Text="Data Entry Type" Name="dataEntryTypeLabel" Align="Fill" TextAlign="Center" FontWeight="Bold"/>
                <ComboBox Grid.Row="2" Grid.Column="1" Name="dataEntryTypeCBox" />

                <Label Grid.Row="1" Grid.Column="3" Text="Channel Type" Align="Fill" TextAlign="Center" FontWeight="Bold"/>
                <ComboBox Grid.Row="2" Grid.Column="3" Name="channelTypeCBox"/>

                <Label Grid.Row="1" Grid.Column="5" Text="Channel" Name="channelSelectionLabel" Align="Fill" TextAlign="Center" FontWeight="Bold"/>
                <ComboBox Grid.Row="2" Grid.Column="5" Name="channelSelectionCBox"/>

                <Label Grid.Row="1" Grid.Column="7" Text="Page #" Name ="pageNumLabel" Align="Fill" TextAlign="Center" FontWeight="Bold"/>
                <TextBox Grid.Row="2" Grid.Column="7" Placeholder="1" Name="pageNumTextBox" ValidateType="Integer"/>

                <Label Grid.Row="1" Grid.Column="9" Text="Cell / Starting Cell" Name="cellRefLabel" Align="Fill" TextAlign="Center" FontWeight="Bold"/>
                <TextBox Grid.Row="2" Grid.Column="9" Placeholder="A1" Name="cellRefTextBox" TextHint="'A1'" />

                <Button Grid.Row="2" Grid.Column="11" Label="Add Channel" Name="addChannelBtn" FontWeight="Bold" Align="Top"/>

            </Grid>
        </CaptionPanel>
        <CaptionPanel Grid.Row="2" Grid.Column="0" Title="Channel List" Name="currentChannelsPanel">
            <Grid>
                <Grid.ColumnDefinitions>
                    <ColumnDefinition Width="20"/>
                    <ColumnDefinition Width="490"/>
                    <ColumnDefinition Width="400"/>
                    <ColumnDefinition Width="100"/>

                </Grid.ColumnDefinitions>

                <Grid.RowDefinitions>
                    <RowDefinition Height="10"/>
                    <RowDefinition Height="Auto"/>
                    <RowDefinition Height="10"/>
                    <RowDefinition Height="Auto" />
                </Grid.RowDefinitions>


                <Button Grid.Column="3" Grid.Row="3" Label="Delete" Name="deleteChannelBtn" FontWeight="Bold"/>
                <ListBox Grid.Row="1" Grid.Column="1" SelectionMode="Single" Name="channelListBox" Grid.ColumnSpan="3" />

            </Grid>
        </CaptionPanel>
    </Grid>
</Window>
  �   P   M U I   U I / S E T T I N G S _ W I N D O W . X M L         0          <?xml version="1.0" encoding="utf-8"?>
<Window xmlns="https://mui.dewesoft.com/schema/1.1" Name="settingsWindow" Background="white">
    <StackPanel>
		<Label Text="Settings"/>
	</StackPanel>	
</Window> l      �� ��     0          l4   V S _ V E R S I O N _ I N F O     ���                 ?                         �   S t r i n g F i l e I n f o   �   0 4 0 9 0 4 b 0   D   F i l e D e s c r i p t i o n     H T T P   R e q u e s t s   0   F i l e V e r s i o n     1 . 0 . 0 . 0   F   I n t e r n a l N a m e   H T T P R e q u e s t s P l u g i n     V   L e g a l C o p y r i g h t   C o p y r i g h t   c   T y l e r   R o b e r g     F   P r o d u c t N a m e     H T T P R e q u e s t s P l u g i n     4   P r o d u c t V e r s i o n   1 . 0 . 0 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	�