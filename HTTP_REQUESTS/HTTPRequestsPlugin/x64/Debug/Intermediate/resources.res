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
  �  H   M U I   U I / S E T U P _ W I N D O W . X M L       0          <?xml version="1.0" encoding="utf-8"?>
<Window xmlns="https://mui.dewesoft.com/schema/1.1">
    <Grid>
        <Grid.ColumnDefinitions>
            <ColumnDefinition Width="100%"/>
        </Grid.ColumnDefinitions>
        <Grid.RowDefinitions>
            <RowDefinition Height="80"/>
            <RowDefinition Height="100%"/>
        </Grid.RowDefinitions>

        <CaptionPanel Grid.Row="0" Grid.Column="0" Title="My panel 2" Name="myPanel1">
            <Grid>
                <Grid.RowDefinitions>
                    <RowDefinition Height="Auto"/>
                    <RowDefinition Height="21"/>
                    <RowDefinition Height="20"/>
                </Grid.RowDefinitions>
                <Grid.ColumnDefinitions>
                    <ColumnDefinition Width="120"/>
                    <ColumnDefinition Width="3"/>
                    <ColumnDefinition Width="120"/>
                    <ColumnDefinition Width="3"/>
                    <ColumnDefinition Width="90"/>
                    <ColumnDefinition Width="90"/>
                </Grid.ColumnDefinitions>

                <Label Grid.Column="0" Grid.Row="0" Text="My TextBox" Name="textBoxLabel"/>
                <TextBox Grid.Row="1" Grid.Column="0" Placeholder="Label text" Name="labelEdit" ValidateType="Integer"/>

                <Label Grid.Row="0" Grid.Column="2" Text="My ComboBox" Name="selectedItemLabel"/>
                <ComboBox Grid.Row="1" Grid.Column="2" Name="comboBox1">
                    <ComboBoxItem Label="My Item 1" />
                    <ComboBoxItem Label="My Item 2" IsSelected="True"/>
                </ComboBox>

                <StackPanel Grid.Row="1" Grid.Column="4" Name="enableDisableStackPanel">

                </StackPanel>

                <Button Grid.Row="1" Grid.Column="5" Name="showModal" Label="Show modal" Width="95"/>
                <CheckBox Grid.Row="2" Grid.Column="0" Name="showHideTextBox" Label="Hide Textbox" IsChecked="False"/>
            </Grid>
        </CaptionPanel>
        <CaptionPanel Grid.Row="1" Grid.Column="0" Title="My Panel 2" Name="myPanel2">
            <Grid>
                <TabControl Name="TabTest" MarginTop="5">
                    <TabItem Header="Radio buttons">
                        <StackPanel PaddingTop="5" PaddingLeft="5">
                            <TextBlock Name="SampleText" Text="My Text: Lorem Ipsum is simply dummy text of the printing and typesetting industry."/>
                            <RadioButton Name="Radio1" Label="My Radio 1"/>
                            <RadioButton Name="Radio2" Label="My Radio 2"/>
                        </StackPanel>
                    </TabItem>
                    <TabItem Header="Images">
                        <StackPanel PaddingTop="5" PaddingLeft="5">
                            <TextBlock Name="ImageText" Text="Sample image..."/>
                        </StackPanel>
                    </TabItem>
                    <TabItem Header="Channels">
                        <DSDrawGrid Name="channelGrid" Align="Fill" MarginTop="5"/>
                    </TabItem>
                </TabControl>
            </Grid>
        </CaptionPanel>
    </Grid>    
</Window>
�   P   M U I   U I / S E T T I N G S _ W I N D O W . X M L         0          <?xml version="1.0" encoding="utf-8"?>
<Window xmlns="https://mui.dewesoft.com/schema/1.1" Name="settingsWindow" Background="white">
    <StackPanel>
		<Label Text="Settings"/>
	</StackPanel>	
</Window> l      �� ��     0          l4   V S _ V E R S I O N _ I N F O     ���                 ?                         �   S t r i n g F i l e I n f o   �   0 4 0 9 0 4 b 0   D   F i l e D e s c r i p t i o n     H T T P   R e q u e s t s   0   F i l e V e r s i o n     1 . 0 . 0 . 0   F   I n t e r n a l N a m e   H T T P R e q u e s t s P l u g i n     V   L e g a l C o p y r i g h t   C o p y r i g h t   c   T y l e r   R o b e r g     F   P r o d u c t N a m e     H T T P R e q u e s t s P l u g i n     4   P r o d u c t V e r s i o n   1 . 0 . 0 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	�