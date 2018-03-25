using Android.App;
using Android.Widget;
using Android.OS;
using Android.Views;
using Android.Content.PM;
using Android.Content;
using System;
using System.Text;

namespace K9Control4
{
    [Activity(Label = "K9", 
        MainLauncher = true, 
        Icon = "@drawable/icon2",
        ScreenOrientation = ScreenOrientation.Landscape,
        ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation 
        
        )]
    public class MainActivity : Activity
    {
        Button btConnect = null;
        Android.Bluetooth.BluetoothSocket k9BTSocket = null;
        int cnt = 1;
        TextView txInfo = null;
        ImageView imgTarget = null;
        Button btSendTest = null;

        private void TouchMeImageViewOnTouch(object sender, View.TouchEventArgs touchEventArgs)
        {
            int turnPosn = 0; // middle
            int directionVal = 0; //stopped

            string message = string.Empty;
            int xVal = (int)touchEventArgs.Event.RawX;
            int yVal = (int)touchEventArgs.Event.RawY;

            string xdirection = string.Empty;
            string ydirection = string.Empty;

            if (xVal < 1660 || yVal < 350)
            {
                txInfo.Text = string.Empty;
                return;
            }

            // get turn value
            if (xVal >= 1960 && xVal <= 2160)
            {
                xdirection = "middle";
                turnPosn = 0;
            }

            if (xVal > 1660 && xVal < 1960)
            { 
                xdirection = "left";
                turnPosn = (11- (10-(1960 - xVal) / 30))*-1;

            }


            if (xVal > 2160 && xVal < 2460)
            {
                xdirection = "right";
                turnPosn = (( (2460  - xVal) / 30) -10) * -1;
            }
           
            // get  direction value
            if (yVal >= 750 && yVal <= 1030)
            {
                ydirection = "stop";
                directionVal = 0; //stopped
            }

            if (yVal > 350 && yVal < 750)
            {
                ydirection = "fwd";
                directionVal = (750 - yVal) /30 ;
            }

            if (yVal > 1030 && yVal < 1330)
            {
                ydirection = "rev";
                directionVal = (33-(yVal  /30)) ;
            }

           // message = string.Format("{0},{1},P:{2},D:{3}",xVal, yVal, turnPosn.ToString(), directionVal.ToString());

            switch (touchEventArgs.Event.Action & Android.Views.MotionEventActions.Mask)
                {
                    case MotionEventActions.Down:
                    case MotionEventActions.Move:
                        //message += "Touch Begins";
                        break;

                    case MotionEventActions.Up:
                        //message += "Touch Ends";
                        {
                        turnPosn = 0; // middle 
                        xdirection = "middle";
                        directionVal = 0; //stopped
                        ydirection = "stop";
                    }
                        break;

                    default:
                        message = string.Empty;
                        break;
                }



            BTSend(GetPositionDirection(turnPosn, directionVal), true);
            txInfo.Text = string.Format("{0}:{1},{2}:{3}",xdirection,turnPosn,ydirection, directionVal);



        }

        private int GetPositionDirection(int turnPosn, int directionVal)
        {
            int returnValue = 0;

            if (turnPosn > 9)   turnPosn = 9;
            if (turnPosn < -9)  turnPosn = -9;

            if (directionVal > 9) directionVal = 9;
            if (directionVal < -9) directionVal = -9;


            //direction setting
            if (directionVal == 0) //stop
            {
                returnValue = 100;
            }
            else
                if (directionVal < 0) // reverse
            {
                returnValue = 200 +  Math.Abs(directionVal);
            }
            else //forward
            {
                returnValue = 300 + Math.Abs(directionVal);
            }


            if (turnPosn == 0) //straight
            {
                //do nothing
            }
            else if (turnPosn < 0) //left
            {
                turnPosn = (turnPosn / 2) * 10; // should only be values up to 40 multiples of 10
                returnValue += Math.Abs(turnPosn);

            }
            else //right
            {
                turnPosn = 40 + (turnPosn / 2) * 10; // should only be values over to 40 multiples of 10
                returnValue += Math.Abs(turnPosn);
            }
            
            return returnValue;
        }

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            //  NavigationPage.SetHasNavigationBar(this, false);
            Window.RequestFeature(WindowFeatures.NoTitle);
            SetContentView (Resource.Layout.Main);
            imgTarget = this.FindViewById<ImageView>(Resource.Id.imgTarget);
            imgTarget.Touch += TouchMeImageViewOnTouch;
            imgTarget.PlaySoundEffect(SoundEffects.Click);

            txInfo = FindViewById<TextView>(Resource.Id.txInfo);

          //  ImageView imgJoystick = this.FindViewById<ImageView>(Resource.Id.imgTarget);

            //** Connection buttons
            btConnect = FindViewById<Button>(Resource.Id.btConnect);
            btConnect.Click += delegate { BTConnect(); };

            Button btDisconnect = FindViewById<Button>(Resource.Id.btDisconnect);
            btDisconnect.Click += delegate { BTDisconnect(); };

            Button btDiagnostic = FindViewById<Button>(Resource.Id.btDiagnostic);
            btDiagnostic.Click += delegate { OpenBTDialog(); };

            //** Actions
            Button btEars = FindViewById<Button>(Resource.Id.btEars);
            btEars.Click += delegate { BTSend(4,true); };

            Button btTail = FindViewById<Button>(Resource.Id.btTail);
            btTail.Click += delegate { BTSend(2, true); };


            Button btLaserIn = FindViewById<Button>(Resource.Id.btLaserIn);
            btLaserIn.Click += delegate { BTSend(10, true); };

            Button btLaserOut = FindViewById<Button>(Resource.Id.btLaserOut);
            btLaserOut.Click += delegate { BTSend(8, true); };


            Button btLaserOnOff = FindViewById<Button>(Resource.Id.btLaserOnOff);
            btLaserOnOff.Click += delegate { BTSend(12, true); };

            //** Sounds

            Button btAffirmative = FindViewById<Button>(Resource.Id.btAffirmative);
            btAffirmative.Click += delegate { BTSend(30, true); };



            Button btNegative = FindViewById<Button>(Resource.Id.btNegative);
            btNegative.Click += delegate { BTSend(31, true); };

            Button btMaster = FindViewById<Button>(Resource.Id.btMaster);
            btMaster.Click += delegate { BTSend(32, true); };


            Button btMistress = FindViewById<Button>(Resource.Id.btMistress);
            btMistress.Click += delegate { BTSend(33, true); };


            Button btNoData = FindViewById<Button>(Resource.Id.btNoData);
            btNoData.Click += delegate { BTSend(34, true); };


            Button btBeGood = FindViewById<Button>(Resource.Id.btBeGood);
            btBeGood.Click += delegate { BTSend(35, true); };


            Button bt1978 = FindViewById<Button>(Resource.Id.bt1978);
            bt1978.Click += delegate { BTSend(36, true); };

            Button btEfficientMachine = FindViewById<Button>(Resource.Id.btEfficientMachine);
            btEfficientMachine.Click += delegate { BTSend(37, true); };


            Button btIAmK9 = FindViewById<Button>(Resource.Id.btIAmK9);
            btIAmK9.Click += delegate { BTSend(38, true); };


            Button btNaturally = FindViewById<Button>(Resource.Id.btNaturally);
            btNaturally.Click += delegate { BTSend(39, true); };

            Button btEyesToggle = FindViewById<Button>(Resource.Id.btEyesToggle);
            btEyesToggle.Click += delegate { BTSend(15, true); };

            Button btEyesFlash = FindViewById<Button>(Resource.Id.btEyesFlash);
            btEyesFlash.Click += delegate { BTSend(16, true); };


            var editText = FindViewById<EditText>(Resource.Id.editText);
            editText.TextChanged += (object sender, Android.Text.TextChangedEventArgs e) => {
                txInfo.Text = e.Text.ToString();

            btSendTest = FindViewById<Button>(Resource.Id.btSendTest);


            btSendTest.Click += delegate {
                    int sendVal;
                    int.TryParse(editText.Text, out sendVal);
                    
                    BTSend(sendVal, true);
             
            };

               


            };

        }

        public void OpenBTDialog()
        {
            StartActivity(new Android.Content.Intent(Android.Provider.Settings.ActionBluetoothSettings));

        }

        private void BTConnect()
        {
            txInfo.Text = "Connecting..";
            btConnect.Text = "Connect";
            K9Bluetooth bt = new K9Bluetooth();
            string message = string.Empty;
            bool IsConnected = bt.Connect(this.ApplicationContext, btConnect, out k9BTSocket, out message);


            if (!IsConnected)
            {
                K9Helper.dialogOKBox("Failed to connect.." + message, this);
                txInfo.Text = "Connect failed";
                btConnect.Text = "Connect";
            }

            txInfo.Text = "Connected.";
        }

        private void BTDisconnect()
        {
            btConnect.Text = "Connect";
            if (k9BTSocket == null)
            {
                K9Helper.dialogOKBox("BT was not connected!", this);

                return;
            }
            k9BTSocket.Close();
            K9Helper.dialogOKBox("Disconnected!", this);
            txInfo.Text = "Disconnected.";
        }

        private void BTSend(int value, bool check, bool debug = false)
        {
           txInfo.Text = "Sending:" + value.ToString();
            if (check && (k9BTSocket == null || !k9BTSocket.IsConnected))
            {
               // K9Helper.dialogOKBox(string.Format("Send failed for {0}. Not connected!", value), this);
                txInfo.Text = string.Format("Send failed for {0}", value);
                return;

            }


            if (k9BTSocket != null)
            {
                // byte[] bdata = System.BitConverter.GetBytes(value);
                // k9BTSocket.OutputStream.WriteAsync(bdata, 0, bdata.Length);
                //k9BTSocket.OutputStream.WriteByte(bdata[0]);
                byte[] bdata = Encoding.ASCII.GetBytes(value.ToString() + "~");

                k9BTSocket.OutputStream.Write(bdata, 0, bdata.Length);

             //   if(value > 99)

               // k9BTSocket.
            }
        }
    }
}

