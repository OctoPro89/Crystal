//Use the Crystal namespace so that we can access entities and components
using Crystal;
//make a namespace to put the class in(optional)
namespace Player
{
    // Here we put our class. The class must dirive from entity
    class TransformController : Entity
    {
        private float speed = 5.0f;
        public TransformComponent tc;

        // Here we make a transform component which we will set t the entitie's transform component that this script is
        //attached to

        //Set it on create
        void OnCreate()
        {
            // Here we use the getcomponent function to get the transform
            tc = GetComponent<TransformComponent>();
        }

        //Here we get input and update the transform
        void OnUpdate(float ts)
        {
            //Create a vector 3 to set the transform to
            Vector3 velocity = Vector3.Zero;

            // We use the IsKeyDown function to see if the wasd keys are pressed, then we change the velocity to 
            // the speed * delta time
            if(Input.IsKeyDown(KeyCode.W))
                velocity.Y += speed * ts;
            if(Input.IsKeyDown(KeyCode.S))
                velocity.Y -= speed * ts;
            if(Input.IsKeyDown(KeyCode.A))
                velocity.X += speed * ts;
            if(Input.IsKeyDown(KeyCode.D))
                velocity.X -= speed * ts;

            tc.Translation = velocity;
        }
    }
}